/*********************************************************************
 *        _       _         _
 *  _ __ | |_  _ | |  __ _ | |__   ___
 * | '__|| __|(_)| | / _` || '_ \ / __|
 * | |   | |_  _ | || (_| || |_) |\__ \
 * |_|    \__|(_)|_| \__,_||_.__/ |___/
 *
 * www.rt-labs.com
 * Copyright 2021 rt-labs AB, Sweden.
 *
 * This software is dual-licensed under GPLv3 and a commercial
 * license. See the file LICENSE.md distributed with this software for
 * full license information.
 ********************************************************************/

#include "app_data.h"
#include "app_gsdml.h"
#include "app_log.h"
#include "osal.h"
#include "pnal.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint8_t discrete_input_data[APP_GSDML_DISCRETE_INPUT_SIZE] = { 0 };
static uint8_t digital_input_coils_data[APP_GSDML_COILS_INPUT_SIZE] = { 0 };
static uint8_t digital_output_coils_data[APP_GSDML_COILS_OUTPUT_SIZE] = { 0 };
static uint8_t analog_input_hold_regs_data[APP_GSDML_HOLDING_INPUT_REGS_SIZE * 4] = { 0 };
static uint8_t analog_output_hold_regs_data[APP_GSDML_HOLDING_OUTPUT_REGS_SIZE * 4] = { 0 };
static uint8_t analog_input_regs_data[APP_GSDML_INPUT_REGS_SIZE * 4] = { 0 };

static void set_mb_digital_buffer(uint8_t *mb_buffer, uint8_t *pf_buffer, uint16_t num_to_read)
{
	memcpy(pf_buffer, mb_buffer, num_to_read);
}

void set_single_read_hold_input_reg_value(cmd_object_t *object, uint32_t address)
{
	if (address > APP_GSDML_HOLDING_INPUT_REGS_SIZE * 4)
		return;
	uint32_t netorder_bytes = 0;
	memcpy(&netorder_bytes, analog_input_hold_regs_data + (address * sizeof(uint32_t)), sizeof(uint32_t));
	uint32_t hostorder_bytes = CC_FROM_BE32(netorder_bytes);
	memcpy(&object->value, &hostorder_bytes, sizeof(hostorder_bytes));
}

static void set_mb_analog_buffer(uint16_t first_addr, uint16_t *mb_buffer, uint8_t *pf_buffer, uint16_t num_to_read)
{	
	for (size_t i = 0; i < num_to_read; i+=2)
	{		
		uint32_t hostorder_float_bytes = (((uint32_t)(mb_buffer[i] << 16)) | mb_buffer[i + 1]) ;
		uint32_t netorder_float_bytes = CC_TO_BE32(hostorder_float_bytes); 
		memcpy(pf_buffer + (((i/2)+(first_addr-1)) * sizeof(uint32_t)), &netorder_float_bytes, sizeof(uint32_t));		
	}	
}

void set_pf_analog_input_data(cmd_object_t  *object, uint16_t *buffer, uint16_t num_to_read, tbl_t *node_tbl)
{
	switch (*node_tbl)
	{
	case HOLD:
		if ((num_to_read/2) > APP_GSDML_HOLDING_INPUT_REGS_SIZE)
			return;
		set_mb_analog_buffer(object->addr, buffer, analog_input_hold_regs_data, num_to_read);
	break;
	case REG:
		if ((num_to_read/2) > APP_GSDML_INPUT_REGS_SIZE)
			return;
		set_mb_analog_buffer(object->addr, buffer, analog_input_regs_data, num_to_read);
	break;
	default:
		break;
	}	
}

int set_single_write_coil_value(cmd_object_t *object, mb_tcp_cfg_t *cfg, uint32_t *base_address, mb_write_cb mb_write)
{	
		uint32_t relative_address_index = object->write_index;
		size_t byte_index = relative_address_index / 8;
		size_t bit_index = relative_address_index % 8;
		
		if (byte_index > APP_GSDML_COILS_OUTPUT_SIZE)
			return -1;
				
		uint32_t updated_value = (digital_output_coils_data[byte_index] & BIT(bit_index)) ? 1 : 0;
		
	if (updated_value != object->value)
	{
		object->value = updated_value;
		uint32_t absolute_address = *base_address + object->addr;
		return mb_write(cfg, &absolute_address, (uint16_t*)&object->value);
	}
	return 0;
}

int set_single_write_holding_reg_value(cmd_object_t *object, mb_tcp_cfg_t *cfg, uint32_t *base_address, mb_write_cb mb_write)
{	
	if (object->write_index > APP_GSDML_HOLDING_OUTPUT_REGS_SIZE * 4)
		return -1;
	
	uint32_t netorder_bytes = 0;
	memcpy(&netorder_bytes, analog_output_hold_regs_data + (object->write_index* sizeof(uint32_t)), sizeof(uint32_t));
	uint32_t updated_value = CC_FROM_BE32(netorder_bytes);
	
	if (updated_value != object->value)
	{
		float float_val;
		object->value = updated_value;
		uint32_t reg1 = *base_address + object->addr;
		uint32_t reg2 = *base_address + object->addr + 1;
		uint16_t mb_write_val_1 = (uint16_t)(updated_value >> 16);
		uint16_t mb_write_val_2 = (uint16_t)(updated_value);
		int sz1 = mb_write(cfg, &reg1, &mb_write_val_1);
		int sz2 = 0;
		if(sz1 > -1)
			sz2 = mb_write(cfg, &reg2, &mb_write_val_2);
		
		return sz1+sz2;
	}
	return 0;
}

void set_single_read_coil_discrete_value(cmd_object_t *object, uint32_t address)
{
		size_t byte_index = address / 8;
		size_t bit_index = address % 8;
		
		if (byte_index > APP_GSDML_COILS_INPUT_SIZE)
			return;
		object->value = (digital_input_coils_data[byte_index] & BIT(bit_index)) ? 1 : 0;
}

void set_pf_digital_input_data(cmd_object_t  *object, uint8_t *buffer, uint16_t num_to_read, tbl_t *node_tbl)
{
	switch (*node_tbl)
	{
		case COIL:
			if (num_to_read > APP_GSDML_COILS_INPUT_SIZE)
				return;
			set_mb_digital_buffer(buffer, digital_input_coils_data, num_to_read);
		break;
		case INPUT:
		if (num_to_read > APP_GSDML_DISCRETE_INPUT_SIZE)
			return;
		set_mb_digital_buffer(buffer, discrete_input_data, num_to_read);
		break;
		default:
			break;
	}
}

 /* Parameter data for digital submodules
  * The stored value is shared between all digital submodules in this example.
  *
  * Todo: Data is always in pnio data format. Add conversion to uint32_t.
  */
static uint32_t app_param_1 = 0; /* Network endianness */
static uint32_t app_param_2 = 0; /* Network endianness */

int app_data_set_default_outputs(void)
{
	return 0;
}

uint8_t* app_data_get_input_data(
    uint16_t slot_nbr,
	uint16_t subslot_nbr,
	uint32_t submodule_id,
	uint16_t* size,
	uint8_t* iops)
{	
	if (size == NULL || iops == NULL)
	{
		return NULL;
	}

	if (submodule_id == APP_GSDML_SUBMOD_ID_DIGITAL_DISCRETE_IN)
	{
		*size = APP_GSDML_DISCRETE_INPUT_SIZE;
		*iops = PNET_IOXS_GOOD;
		return discrete_input_data;
	}
	
	if (submodule_id == APP_GSDML_SUBMOD_ID_DIGITAL_COILS_IN_OUT)
	{	
		*size = APP_GSDML_COILS_INPUT_SIZE;
		*iops = PNET_IOXS_GOOD;
		return digital_input_coils_data;
	}

	if (submodule_id == APP_GSDML_SUBMOD_ID_HOLDING_REGISTER)
	{
		*size = APP_GSDML_HOLDING_INPUT_REGS_SIZE * 4;
		*iops = PNET_IOXS_GOOD;
		return analog_input_hold_regs_data;
	}
	
	if (submodule_id == APP_GSDML_SUBMOD_ID_INPUT_REGISTER)
	{
		*size = APP_GSDML_INPUT_REGS_SIZE * 4;
		*iops = PNET_IOXS_GOOD;
		return analog_input_regs_data;
	}
	*iops = PNET_IOXS_BAD;
	return NULL;
}


int app_data_write_parameter(
    uint16_t slot_nbr,
    uint16_t subslot_nbr,
    uint32_t submodule_id,
    uint32_t index,
    const uint8_t* data,
    uint16_t length)
{
    const app_gsdml_param_t* par_cfg;

    par_cfg = app_gsdml_get_parameter_cfg(submodule_id, index);
    if (par_cfg == NULL)
    {
        APP_LOG_WARNING(
            "PLC write request unsupported submodule/parameter. "
            "Submodule id: %u Index: %u\n",
            (unsigned)submodule_id,
            (unsigned)index);
        return -1;
    }

    if (length != par_cfg->length)
    {
        APP_LOG_WARNING(
            "PLC write request unsupported length. "
            "Index: %u Length: %u Expected length: %u\n",
            (unsigned)index,
            (unsigned)length,
            par_cfg->length);
        return -1;
    }

    if (index == APP_GSDML_PARAMETER_1_IDX)
    {	    
        memcpy(&app_param_1, data, length);
    }
    else if (index == APP_GSDML_PARAMETER_2_IDX)
    {
        memcpy(&app_param_2, data, length);
    }
    else if (index == APP_GSDML_PARAMETER_ECHO_IDX)
    {
        //memcpy(&app_param_echo_gain, data, length);
    }

    APP_LOG_DEBUG("  Writing parameter \"%s\"\n", par_cfg->name);
    app_log_print_bytes(APP_LOG_LEVEL_DEBUG, data, length);

    return 0;
}
int app_data_read_parameter(
	uint16_t slot_nbr,
	uint16_t subslot_nbr,
	uint32_t submodule_id,
	uint32_t index,
	uint8_t** data,
	uint16_t* length)
{
	const app_gsdml_param_t* par_cfg;

	par_cfg = app_gsdml_get_parameter_cfg(submodule_id, index);
	if (par_cfg == NULL)
	{
		APP_LOG_WARNING(
			"PLC read request unsupported submodule/parameter. "
			"Submodule id: %u Index: %u\n",
			(unsigned)submodule_id,
			(unsigned)index);
		return -1;
	}

	if (*length < par_cfg->length)
	{
		APP_LOG_WARNING(
			"PLC read request unsupported length. "
			"Index: %u Length: %u Expected length: %u\n",
			(unsigned)index,
			(unsigned)* length,
			par_cfg->length);
		return -1;
	}

	APP_LOG_DEBUG("  Reading \"%s\"\n", par_cfg->name);
	if (index == APP_GSDML_PARAMETER_1_IDX)
	{
		*data = (uint8_t*)& app_param_1;
		*length = sizeof(app_param_1);
	}
	else if (index == APP_GSDML_PARAMETER_2_IDX)
	{
		*data = (uint8_t*)& app_param_2;
		*length = sizeof(app_param_2);
	}
	else if (index == APP_GSDML_PARAMETER_ECHO_IDX)
	{
		//*data = (uint8_t*)& app_param_echo_gain;
		//*length = sizeof(app_param_echo_gain);
	}

	app_log_print_bytes(APP_LOG_LEVEL_DEBUG, *data, *length);

	return 0;
}

int app_data_set_output_data(
	uint16_t slot_nbr,
	uint16_t subslot_nbr,
	uint32_t submodule_id,
	uint8_t* data,
	uint16_t size)
{
	if (data == NULL)
	{
		return -1;
	}

	if (submodule_id == APP_GSDML_SUBMOD_ID_DIGITAL_COILS_IN_OUT)
	{
		if (size == APP_GSDML_COILS_OUTPUT_SIZE)
		{			
			memcpy(digital_output_coils_data, data, APP_GSDML_COILS_OUTPUT_SIZE);			
			return 0;
		}
	}
	else if (submodule_id == APP_GSDML_SUBMOD_ID_HOLDING_REGISTER)
	{
		if (size == (APP_GSDML_HOLDING_OUTPUT_REGS_SIZE * 4))
		{
			memcpy(analog_output_hold_regs_data, data, APP_GSDML_HOLDING_OUTPUT_REGS_SIZE * 4);
			return 0;
		}
	}

	return -1;
}

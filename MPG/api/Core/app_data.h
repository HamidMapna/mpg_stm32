/*********************************************************************
 *        _       _         _
 *  _ __ | |_  _ | |  __ _ | |__   ___
 * | '__|| __|(_)| | / _` || '_ \ / __|
 * | |   | |_  _ | || (_| || |_) |\__ \
 * |_|    \__|(_)|_| \__,_||_.__/ |___/
 *
 * www.rt-labs.com
 * Copyright 2018 rt-labs AB, Sweden.
 *
 * This software is dual-licensed under GPLv3 and a commercial
 * license. See the file LICENSE.md distributed with this software for
 * full license information.
 ********************************************************************/

#ifndef APP_DATA_H
#define APP_DATA_H

 /**
  * @file
  * @brief Sample application data interface
  *
  * Functions for:
  * - Getting input data (Button 1 and counter value)
  * - Setting output data (LED 1)
  * - Setting default output state. This should be
  *   part of all device implementations for setting
  *   defined state when device is not connected to PLC
  * - Reading and writing parameters
  */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "xml.h"

	typedef int(*mb_write_cb)(mb_tcp_cfg_t *, uint32_t*, uint16_t*);
	
	void set_pf_analog_input_data(cmd_object_t  *object, uint16_t *buffer, uint16_t num_to_read, tbl_t *node_tbl);
	void set_pf_digital_input_data(cmd_object_t  *object, uint8_t *buffer, uint16_t num_to_read, tbl_t *node_tbl);
	int set_single_write_coil_value(cmd_object_t *object, mb_tcp_cfg_t *cfg, uint32_t *base_address, mb_write_cb mb_write_callback);
	int set_single_write_holding_reg_value(cmd_object_t *object, mb_tcp_cfg_t *cfg, uint32_t *base_address, mb_write_cb mb_write_callback);
	void set_single_read_coil_discrete_value(cmd_object_t *object, uint32_t address);
	void set_single_read_hold_input_reg_value(cmd_object_t *object, uint32_t address);
	int app_data_set_default_outputs(void);

	uint8_t * app_data_get_input_data(
	   uint16_t slot_nbr,
		uint16_t subslot_nbr,
		uint32_t submodule_id,		
		uint16_t * size,
		uint8_t * iops);
	
	int app_data_write_parameter(
		uint16_t slot_nbr,
		uint16_t subslot_nbr,
		uint32_t submodule_id,
		uint32_t index,
		const uint8_t* data,
		uint16_t write_length);
	
	int app_data_read_parameter(
		uint16_t slot_nbr,
		uint16_t subslot_nbr,
		uint32_t submodule_id,
		uint32_t index,
		uint8_t** data,
		uint16_t* length);

	int app_data_set_output_data(
		uint16_t slot_nbr,
		uint16_t subslot_nbr,
		uint32_t submodule_id,
		uint8_t* data,
		uint16_t size);

#ifdef __cplusplus
}
#endif

#endif /* APP_DATA_H */

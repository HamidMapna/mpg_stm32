#include "mb_tcp_master.h"
#include "app_data.h"
#include "mbus.h"
#include "xml.h"
#include "mb_slave.h"
#include "mb_tcp_slave.h"
#include "osal.h"

#define APP_MAIN_SLEEPTIME_US 5000 * 1000
#define MB_REQUEST_STEP		  124
#define BUFFER_SIZE_BYTE	  (MB_REQUEST_STEP * 2) + 1

static int mb_write_callback(mb_tcp_cfg_t *cfg, mb_address_t *address, uint16_t *value)
{
		return mbus_write_single((mbus_t*)cfg->bus, cfg->slave_hdl, *address, *value);
}

static void traverse_holds(Common_node_t **node_list, mb_address_t *base_address, uint16_t node_number, mb_tcp_cfg_t *cfg)
{
	for (size_t step = 0; *node_list != NULL && step < node_number; step++)
	{
		cmd_object_t *cmd_object = (cmd_object_t *)(*node_list)->object;   

		if (cmd_object->cmd == WRITE)
		{		
			set_single_write_holding_reg_value(cmd_object, cfg, base_address, mb_write_callback);		
		}
		*node_list = (*node_list)->next;
	}	
}

static void traverse_coils(Common_node_t **node_list, mb_address_t *base_address, uint16_t node_number, mb_tcp_cfg_t *cfg)
{
	for (size_t step = 0; *node_list != NULL && step < node_number; step++)
	{
		cmd_object_t *cmd_object = (cmd_object_t *)(*node_list)->object;   
		
		if (cmd_object->cmd == WRITE)
		{			
			set_single_write_coil_value(cmd_object, cfg, base_address, mb_write_callback);		
		}			
		*node_list = (*node_list)->next;
	}
}

static void monitor_slave_analogs(Common_node_t *node_list, int node_number, tbl_t node_tbl, slave_t *slave_obj)
{
	Common_node_t *temp_node_list = node_list;
	
	while (node_list && (node_number > 0))
	{
		cmd_object_t *cmd_object = (cmd_object_t *)node_list->object;   
		int error = 0;
		mb_address_t base_address = MB_ADDRESS(node_tbl, (2*cmd_object->addr)-1);
		uint16_t num_to_read = ((node_number * 2) > MB_REQUEST_STEP) ? MB_REQUEST_STEP : node_number * 2;
		uint16_t holds_buffer[BUFFER_SIZE_BYTE] = { '\0' };
		error = mbus_read((mbus_t *)slave_obj->cfg.bus, slave_obj->cfg.slave_hdl, base_address, num_to_read, holds_buffer);	         	      
						
		if (error == 0)
		{
			set_pf_analog_input_data(cmd_object, holds_buffer, num_to_read, &node_tbl);
			traverse_holds(&node_list, &base_address, MIN(node_number, MB_REQUEST_STEP/2), &slave_obj->cfg);	
			node_number -= (num_to_read/2);
		}			
		if (error == EFRAME_NOK)
		{
			printf("slave %d: connection closed.\n", slave_obj->slave_id);  
			slave_obj->cfg.slave_hdl = -1;
			break;
		}
		else if (error != 0)
		{
			break;
		}			  
	}
}
static void monitor_slave_digitals(Common_node_t *node_list, uint16_t node_number, tbl_t node_tbl, slave_t *slave_obj)
{
	Common_node_t *temp_node_list = node_list;
	
	while (node_list && (node_number > 0))
	{
		cmd_object_t *cmd_object = (cmd_object_t *)node_list->object;   
		int error = 0;
		mb_address_t base_address = MB_ADDRESS(node_tbl, cmd_object->addr);
		
		uint16_t num_to_read = (node_number > MB_REQUEST_STEP) ? MB_REQUEST_STEP: node_number;
		uint8_t coils_buffer[BUFFER_SIZE_BYTE / 8] = { '\0' };
		error = mbus_read((mbus_t *)slave_obj->cfg.bus, slave_obj->cfg.slave_hdl, base_address, num_to_read, coils_buffer);	         	      
			
		if (error == 0)
		{
			set_pf_digital_input_data(cmd_object, coils_buffer, (num_to_read / 8), &node_tbl);
			traverse_coils(&node_list, &base_address, MIN(node_number, MB_REQUEST_STEP), &slave_obj->cfg);
			node_number -= num_to_read;
		}
		
		if (error == EFRAME_NOK)
		{
			printf("slave %d: connection closed.\n", slave_obj->slave_id);  
			slave_obj->cfg.slave_hdl = -1;
			break;
		}
		else if(error != 0)
		{
			break;
		}			  
	}
}	

static void mb_slave_thread(void *arg)
{
  slave_t *slave = arg;		
	while(true)
  {
	  if (slave->cfg.slave_hdl == -1)
	  {
        slave->cfg.slave_hdl = mbus_connect(slave->cfg.bus, slave->ip);	
		  if (slave->cfg.slave_hdl > 0)
		     printf("slave %d: connection established.\n", slave->slave_id);  
	  }  
	  if (slave->cfg.slave_hdl > 0)
	  {
		  monitor_slave_digitals(slave->coils_list.data_nodes, slave->coils_list.number, COIL, slave);
		  monitor_slave_analogs(slave->hold_regs_list.data_nodes, slave->hold_regs_list.number,HOLD, slave);
		  monitor_slave_digitals(slave->input_dsc_list.data_nodes, slave->input_dsc_list.number, INPUT, slave);
		  monitor_slave_analogs(slave->input_regs_list.data_nodes, slave->input_regs_list.number, REG, slave);
	  }
    os_usleep(10);
  }
}

static void init_slaves (database_t *main_db)
{
   static mbus_cfg_t mb_master_cfg = {
      .timeout = 500,
   };
   Common_node_t * slave_list_head = main_db->slave_list;

   while (slave_list_head)
   {
      mb_transport_t * curr_tcp;
      slave_t * curr_slave = (slave_t *)slave_list_head->object;
      curr_tcp = mb_tcp_init (&curr_slave->cfg);
      curr_slave->cfg.bus = (void *)mbus_create (&mb_master_cfg, curr_tcp);     
      curr_slave->cfg.slave_hdl = -1;
	  /* monitor slave tasks*/
      char slave_id[10] = {'\0'};
      os_thread_create (
         slave_id,
         curr_slave->cfg.priority,
         curr_slave->cfg.stack_size,
         mb_slave_thread,
         curr_slave);
      slave_list_head = slave_list_head->next;
   }
}

mb_slave_t *init_master(database_t * main_db)
{
    mb_slave_t *slave;
    mb_transport_t *tcp;
    static const mb_tcp_cfg_t mb_tcp_cfg = {
        .port = 502,
    };

    tcp = mb_tcp_init(&mb_tcp_cfg);
    slave = mb_slave_init (&mb_slave_cfg_main, tcp, main_db);
    return slave;
  }
 
void init_modbus_tcp()
{
   database_t *main_db= load_database();
   init_slaves (main_db);
  // init_master(main_db);
}


#include "mb_tcp_slave.h"
#include "mb_tcp.h"
#include "mb_rtu.h"
#include "osal.h"
#include <string.h>

static int coil_get(uint16_t address, uint8_t *data, node_list_t *data_list, size_t quantity)
{
   uint16_t offset;

   for (offset = 0; offset < quantity; offset++)
   {
      uint32_t bit = address + offset;

      if(bit >= (data_list->number * 8))//out of available address range
        return EILLEGAL_DATA_ADDRESS;
      
      int value;
      value = mb_slave_bit_get_from_node_list(data_list, bit);
      mb_slave_bit_set_char_array(data, offset, value);
   }
   return 0;
}

static int coil_set(uint16_t address, uint8_t *data, node_list_t *data_list, size_t quantity)
{
   uint16_t offset;

   for (offset = 0; offset < quantity; offset++)
   {
      uint32_t bit = address + offset;

      if (bit >= (data_list->number * 8)) // out of available address range
        return EILLEGAL_DATA_ADDRESS;
      
      int value;
      value = mb_slave_bit_get_from_char_array(data, offset);
      mb_slave_bit_set_node_list(data_list, bit, value);
   }
   return 0;
}

static int input_get(uint16_t address, uint8_t *data, node_list_t *data_list, size_t quantity)
{
   uint16_t offset;

   for (offset = 0; offset < quantity; offset++)
   {
      uint32_t bit = address + offset;
      
      if(bit >= (data_list->number * 8))
        return EILLEGAL_DATA_ADDRESS;

      int value = mb_slave_bit_get_from_node_list(data_list, bit);
      mb_slave_bit_set_char_array(data, offset, value);
   }
   return 0;
}

static int hold_get(uint16_t address, uint8_t *data, node_list_t *data_list, size_t quantity)
{
   uint16_t offset;

   for (offset = 0; offset < quantity; offset++)
   {
      uint32_t reg = address + offset;
      
      if (reg >= data_list->number)
        return EILLEGAL_DATA_ADDRESS;

      uint16_t value = mb_slave_reg_get_from_node_list(data_list, reg);
      mb_slave_reg_set(data, offset, value);      
   }
   return 0;
}

static int hold_set(uint16_t address, uint8_t *data, node_list_t *data_list, size_t quantity)
{
   uint16_t offset;

   for (offset = 0; offset < quantity; offset++)
   {
      uint32_t reg = address + offset;
      
      if (reg >= data_list->number)
        return EILLEGAL_DATA_ADDRESS;
      
      uint16_t value = mb_slave_reg_get_from_char_array(data, offset);
      mb_slave_reg_set_node_list(data_list, reg, value);
   }
   return 0;
}

static int reg_get(uint16_t address, uint8_t *data, node_list_t *data_list, size_t quantity)
{
   uint16_t offset;

   for (offset = 0; offset < quantity; offset++)
   {
     uint32_t reg = address + offset;
     
     if(reg >= data_list->number )
       return EILLEGAL_DATA_ADDRESS;

     uint16_t value = mb_slave_reg_get_from_node_list(data_list, reg);
     mb_slave_reg_set(data, offset, value);
   }
   return 0;
}

static int ping (uint8_t * data, size_t rx_count)
{
   char * message = "Hello World";
   memcpy (data, message, strlen (message));
   return (int)strlen (message);
}

static const mb_vendor_func_t vendor_funcs[] = {
   {101, ping},
};

const mb_iomap_t mb_slave_iomap = {
    .coils = {1000, coil_get, coil_set},             // support 1000 coils
    .inputs = {1000, input_get, NULL},              // support 1000  input status bits
    .holding_registers = {100, hold_get, hold_set},  // support 100 holding registers
    .input_registers   = {100, reg_get, NULL},       // support 100 input registers
    .num_vendor_funcs  = NELEMENTS (vendor_funcs),   // 1 vendor function
    .vendor_funcs      = vendor_funcs,
};

const mb_slave_cfg_t mb_slave_cfg_main = {
   .id         = 2, // Slave ID: 2
   .priority   = 15,
   .stack_size = 2048,
   .iomap      = &mb_slave_iomap
};

/*********************************************************************
 *        _       _         _
 *  _ __ | |_  _ | |  __ _ | |__   ___
 * | '__|| __|(_)| | / _` || '_ \ / __|
 * | |   | |_  _ | || (_| || |_) |\__ \
 * |_|    \__|(_)|_| \__,_||_.__/ |___/
 *
 * www.rt-labs.com
 * Copyright 2012 rt-labs AB, Sweden.
 *
 * This software is dual-licensed under GPLv3 and a commercial
 * license. See the file LICENSE.md distributed with this software for
 * full license information.
 ********************************************************************/

/**
 * \addtogroup mb_slave Modbus slave
 * \{
 */

#ifndef MB_SLAVE_H
#define MB_SLAVE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mb_transport.h"
#include "mb_error.h"

#include "mb_export.h"
#include "xml.h"

typedef struct mb_iotable
{
   /**
    * Number of valid addresses in table.
    */
   size_t size;

   /**
    * Get callback. This function is called in response to a read
    * operation. The function should perform the get operation by
    * storing the requested data in the \a data array.
    *
    * The mb_slave_bit_set() and mb_slave_reg_set() functions can be
    * used to set the contents of the data array.
    *
    * This callback is not used for input status and input register
    * tables, and should then be set to NULL.
    *
    * The address supplied to the input parameter is 0-based.
    *
    * The callback can return a modbus exception if an error
    * occurs. See mb_error.h for available exceptions.
    *
    * \param address            Starting address
    * \param data               Data array
    * \param quantity           Number of addresses to set
    *
    * \return 0 on success, modbus exception otherwise
    */
   int (*get)(uint16_t address, uint8_t *data, node_list_t *data_list, size_t quantity);

   /**
    * Set callback. This function is called in response to a write
    * operation. The function should perform the set operation using
    * the data provided in the \a data array.
    *
    * The mb_slave_bit_get() and mb_slave_reg_get() functions can be
    * used to extract the contents of the data array.
    *
    * The addresses supplied to the input parameter is 0-based.
    *
    * The callback can return a modbus exception if an error
    * occurs. See mb_error.h for available exceptions.
    *
    * \param address            Starting address
    * \param data               Data array
    * \param quantity           Number of addresses to set
    *
    * \return 0 on success, modbus exception otherwise
    */
   int (*set)(uint16_t address, uint8_t *data, node_list_t *data_list, size_t quantity);

} mb_iotable_t;

typedef struct mb_vendor_func
{
   /**
    * Vendor-defined function code.
    */
   uint8_t function;

   /**
    * Function callback. This function is called in response to
    * receiving the function code specified in \a function.
    *
    * The \a data array holds the received request data. The number of
    * valid bytes in \a data is given by \a size. The first byte of the
    * data is the function code.
    *
    * The callback may return a response by storing it in the \a data
    * array. The callback should return the number of valid bytes in
    * the response, or 0 if no response is to be sent. The response
    * should normally include the function code unchanged.
    *
    * The callback can return a modbus exception if an error
    * occurs. See mb_error.h for available exceptions.
    *
    * An extended exception can be sent by manually setting the MSB of
    * the function code (\a data[0]), and following it with the
    * exception data, returning the total number of valid bytes.
    *
    * \param data               Request data, incl. function code
    * \param size               Size of request data (bytes)
    *
    * \return Size of response on success, modbus exception otherwise
    */
   int (*callback) (uint8_t * data, size_t size);

} mb_vendor_func_t;

typedef struct mb_iomap
{
   mb_iotable_t coils;             /**< Coil definitions */
   mb_iotable_t inputs;            /**< Input status definitions */
   mb_iotable_t holding_registers; /**< Holding register definitions */
   mb_iotable_t input_registers;   /**< Input register definitions */
   size_t num_vendor_funcs;        /**< Number of vendor-defined functions */
   const mb_vendor_func_t * vendor_funcs; /**< Vendor-defined functions */
} mb_iomap_t;

typedef struct mb_slave_cfg
{
   uint8_t id;               /**< Slave ID */
   uint32_t priority;        /**< Priority of slave task */
   size_t stack_size;        /**< Stack size of slave task*/
   const mb_iomap_t * iomap; /**< Slave iomap */
} mb_slave_cfg_t;

typedef struct mb_slave
{
   uint8_t id;
   int running;
   mb_transport_t * transport;
   database_t * db;
   const mb_iomap_t * iomap;
} mb_slave_t;

MB_EXPORT mb_slave_t * mb_slave_init (   
    const mb_slave_cfg_t*,
    mb_transport_t * transport,
   database_t * main_db
);

/**
 * Shutdown a running slave.
 *
 * The slave will exit after serving pending requests.
 *
 * \param slave         slave handle
 */
MB_EXPORT void mb_slave_shutdown (mb_slave_t * slave);

/**
 * Return a handle to the transport data layer.
 *
 * \param slave         slave handle
 *
 * \return handle
 */
MB_EXPORT void * mb_slave_transport_get (mb_slave_t * slave);

/**
 * Change the slave ID.
 *
 * \param slave         handle
 * \param id            new ID
 */
MB_EXPORT void mb_slave_id_set (mb_slave_t * slave, uint8_t id);

/**
 * Get a bit in the bit-string \a data
 *
 * This function gets the bit with index \a address in the bit-string
 * \a data. The function is intended for use by the coils- and
 * inputs callbacks.
 *
 * \param data          bit-string
 * \param address       0-based index of bit to set
 *
 * \return 0 if bit is clear, 1 if bit is set
 */
MB_EXPORT int mb_slave_bit_get_from_node_list(node_list_t * data, uint32_t address);

MB_EXPORT int mb_slave_bit_get_from_char_array(void* data, uint32_t address);

/**
 * Set a bit in the bit-string \a data
 *
 * This function sets the bit with index \a address in the bit-string
 * \a data. The function is intended for use by the coils- and inputs
 * callbacks.
 *
 * \param data          bit-string
 * \param address       0-based index of bit to set
 * \param value         new value (0 to clear, non-zero to set)
 */
MB_EXPORT void mb_slave_bit_set_char_array(void * data, uint32_t address, int value);
MB_EXPORT void mb_slave_bit_set_node_list(node_list_t *data_list, uint32_t address, int value);

/**
 * Get register value from modbus data.
 *
 * This function gets the register with index \a address in the \a
 * data array. The data will be converted from network byte ordering
 * if required. The function is intended for use by the
 * holding_registers.set and input_registers.set callbacks.
 *
 * \param data          register array
 * \param address       0-based index of register to get
 *
 * \return register value
 */
MB_EXPORT uint16_t mb_slave_reg_get_from_node_list(node_list_t * node_list, uint32_t address);
MB_EXPORT uint16_t mb_slave_reg_get_from_char_array(void *data, uint32_t address);

/**
 * Set register value in modbus data.
 *
 * This function sets the register with index \a address in the \a
 * data array. The data is converted to network byte ordering if
 * required. The function is intended for use by the
 * holding_registers.get and input_registers.get callbacks.
 *
 * \param data          register array
 * \param address       0-based index of register to set
 * \param value         new value
 */
MB_EXPORT void mb_slave_reg_set (void * data, uint32_t address, uint16_t value);
MB_EXPORT void mb_slave_reg_set_node_list(node_list_t *node_list, uint32_t address, uint16_t value);

/**
 * \internal
 */
void mb_slave_handle_request (mb_slave_t * slave, pdu_txn_t * transaction);

#ifdef __cplusplus
}
#endif

#endif /* MB_SLAVE_H */

/**
 * \}
 */

#ifndef XML_H
#define XML_H

#include "mb_tcp.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

typedef enum
{
	READ,
	WRITE
}cmd_t;

typedef enum
{
   COIL = 0,
   INPUT = 1,
   REG = 3,
   HOLD = 4,
  UNKNOWN = 5
}tbl_t;

typedef struct master_address
{
  uint16_t addr;
} master_address_t;

typedef struct cmd_object {
  cmd_t cmd;
  uint16_t addr;
  uint32_t value;
  size_t write_index;
} cmd_object_t;

typedef struct Common_node
{
   void * object;
   struct Common_node *next;
}Common_node_t;

typedef struct node_list{
  Common_node_t *data_nodes;
  uint16_t number;
} node_list_t;

typedef struct slave
{
   char		ip[20];
   mb_tcp_cfg_t cfg;
   int slave_id;
   node_list_t	coils_list;
   node_list_t	input_dsc_list;
   node_list_t  input_regs_list;
   node_list_t  hold_regs_list;
   size_t write_coil_index;
   size_t write_holding_index;
}slave_t;

typedef struct master{
  node_list_t coils_list;
  node_list_t input_dsc_list;
  node_list_t input_regs_list;
  node_list_t hold_regs_list;
} master_t;

typedef struct database database_t;

struct database
{
   int slaves_number;
   Common_node_t *slave_list;
   Common_node_t *master_node;
};

database_t *load_database();
#endif
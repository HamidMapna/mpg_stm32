#include "xml.h"
#include <libxml/tree.h>
#include <libxml/parser.h>

xmlDocPtr document = NULL;
static void deallocate_node(Common_node_t **node) {
  (*node)->next = NULL;
  free(*node);
  *node = NULL;
}

static void *remove_common_node_from_linklist(Common_node_t **start_node, Common_node_t *target_node) {
  
  void* object = target_node->object;

  Common_node_t *temp = *start_node;

  while (temp->next != target_node)
    temp = temp->next;

  if (temp != NULL)
  {
    temp->next = target_node->next;
    deallocate_node(&target_node);
  }
  return object;
}

static Common_node_t *add_common_node_to_linklist(Common_node_t **start_node, void * object)
{  
  if (object)
  {
    Common_node_t *node = malloc(sizeof(struct Common_node));    
    if (node) 
    {
      node->object = object;
      node->next = NULL;

      if (*start_node == NULL) 
      {
        *start_node = node;
      }
      else
      {
        Common_node_t *temp = *start_node;
        while (temp->next) 
        {
          temp = temp->next;
        }
        temp->next = node;
      }
    }
    return node;
  }
  return NULL;
}

static xmlDocPtr get_doc_element(const char* db_file) {
  xmlDocPtr doc = xmlParseFile(db_file);
  if (doc == NULL) 
  {
    printf("Failed to parse xml file.\n");
    return NULL;
  }
  return doc;
}

static xmlNodePtr get_root_element(xmlDocPtr doc) {
  xmlNodePtr root_elem = xmlDocGetRootElement(doc);
  if (root_elem == NULL) 
  {
    printf("Failed to get root of xml file.\n");
    return NULL;
  }
  return root_elem;
}

static void getXMLValue(xmlNode *root, const char *parentTag, const char *childTag, char *value) 
{  //level 1
  for (xmlNode *node = root->children; node; node = node->next) 
  {
    if (node->type == XML_ELEMENT_NODE && !xmlStrcmp(node->name, (const xmlChar *)parentTag)) 
    {
      if (strlen(childTag) == 0) 
      {
        strcpy(value, (char *)xmlNodeGetContent(node));
        return;
      }//level 2
      for (xmlNode *childNode = node->children; childNode; childNode = childNode->next) 
      {
        if (childNode->type == XML_ELEMENT_NODE && !xmlStrcmp(childNode->name, (const xmlChar *)childTag))
        {
          strcpy(value, (char *)xmlNodeGetContent(childNode));
          return;
        }
      }
    }
  }
}

static xmlNodePtr get_root()
{
  xmlNodePtr root = NULL;
  char db_file[PATH_MAX] = {'\0'};
  strcpy(db_file, DATABASE_FILE);

  if ((document = get_doc_element(db_file)) == NULL) {
    printf("database document could not be loaded. exit the program");
    exit(EXIT_FAILURE);
  }

  if ((root = get_root_element(document)) == NULL) {
    printf("database failed to get root. exit the program\n");
    exit(EXIT_FAILURE);
  }
  return root;
}
/*
static void getXMLValue_tbl(xmlNode *slave_node, const char *target_key_l1, const char *target_key_l2, tbl_t *target_value) {
  char value[20] = {'\0'};
  char *endptr;
  getXMLValue(slave_node, target_key_l1, target_key_l2, value);
  *target_value = (!strcmp(value, "coil"))
                      ? COIL
                      : (!strcmp(value, "input"))
                            ? INPUT
                            : (!strcmp(value, "reg"))
                                  ? REG
                                  : (!strcmp(value, "hold")) ? HOLD : UNKNOWN;
}
*/
static void getXMLValue_cmd(xmlNode *slave_node, const char *target_key_l1, const char *target_key_l2, cmd_t *target_value) {
  char value[20] = {'\0'};
  char *endptr;
  getXMLValue(slave_node, target_key_l1, target_key_l2, value);
  *target_value = (!strcmp(value, "read")) ? READ : WRITE;
}

static void getXMLValue_hex(xmlNode *slave_node, const char *target_key_l1, const char *target_key_l2, int *target_value) {
  char value[20] = {'\0'};
  char *endptr;
  getXMLValue(slave_node, target_key_l1, target_key_l2, value);
  *target_value = strtoul(value, &endptr, 16);
}

static void getXMLValue_dbl(xmlNode *slave_node, const char *target_key_l1, const char *target_key_l2, uint32_t *target_value) {
  char value[20] = {'\0'};
  char *endptr;
  getXMLValue(slave_node, target_key_l1, target_key_l2, value);
  *target_value = (strlen(value) > 0)? strtod(value, &endptr): 0;
}

static void getXMLValue_ul(xmlNode *slave_node, const char *target_key_l1, const char *target_key_l2, int *target_value)
{
  char value[20] = {'\0'};
  char *endptr;
  getXMLValue(slave_node, target_key_l1, target_key_l2, value);
  *target_value = strtoul(value, &endptr, 10);
}

static void set_node_write_index(slave_t *slave,tbl_t node_type, cmd_object_t *object)
{
	if (object->cmd == WRITE)
	{
		if (node_type == COIL)
			object->write_index = slave->write_coil_index++;
		else if (node_type == HOLD)
			object->write_index = slave->write_holding_index++;
		else
			object->write_index = -1;
	}
}

static void extract_slave_node_content(slave_t *slave, Common_node_t **slave_content_node, uint16_t *number,tbl_t node_type, xmlNode *slave_node)
{
  for (xmlNode *input_desc = slave_node->children; input_desc; input_desc = input_desc->next)
   {
      if (input_desc->type == XML_ELEMENT_NODE)
      {
         cmd_object_t * object = malloc (sizeof (struct cmd_object));
         if (object == NULL)
         {
            printf ("failed to allocate for input descrete object.\n");
            exit (EXIT_FAILURE);
         }
        // getXMLValue_ul (input_desc, "repeat", "", &object->repeat);
         //getXMLValue_tbl (input_desc, "table", "", &object->tbl);
         getXMLValue_cmd (input_desc, "cmd", "", &object->cmd);
         getXMLValue_hex (input_desc, "address", "", (int *)&object->addr);
         //getXMLValue_dbl (input_desc, "value", "", &object->value);
         add_common_node_to_linklist(slave_content_node, (void *)object);
	     set_node_write_index(slave, node_type, object); 
         (*number)++;
      }
   }  
}

static void extract_slave_input_regs(slave_t * slave, xmlNode * inputs_reg)
{
  slave->input_regs_list.number = 0;
  slave->input_regs_list.data_nodes = NULL;
  extract_slave_node_content(slave, &slave->input_regs_list.data_nodes, &slave->input_regs_list.number, REG, inputs_reg);
}

static void extract_slave_holds(slave_t *slave, xmlNode *holds)
{
  slave->hold_regs_list.number = 0;
  slave->hold_regs_list.data_nodes = NULL;
  extract_slave_node_content(slave, &slave->hold_regs_list.data_nodes, &slave->hold_regs_list.number,HOLD, holds);
}

static void extract_slave_input_desc(slave_t *slave, xmlNode *inputs_desc)
{
  slave->input_dsc_list.number = 0;
  slave->input_dsc_list.data_nodes = NULL;
  extract_slave_node_content(slave, &slave->input_dsc_list.data_nodes, &slave->input_dsc_list.number, INPUT, inputs_desc);
}

static void extract_slave_coils(slave_t *slave, xmlNode *coils)
{
  slave->coils_list.number = 0;
  slave->coils_list.data_nodes = NULL;
  extract_slave_node_content(slave, &slave->coils_list.data_nodes, &slave->coils_list.number, COIL, coils);
}

static void extract_slave_content(slave_t *slave, xmlNode *slave_node)
{
	slave->write_coil_index = 0;
	slave->write_holding_index = 0;
  for (xmlNode *slave_childs = slave_node->children; slave_childs; slave_childs = slave_childs->next)
  {
    if (slave_childs->type == XML_ELEMENT_NODE) 
      {
      if (!xmlStrcmp(slave_childs->name, (const xmlChar *)"coils"))
       {
        extract_slave_coils(slave, slave_childs);        
       }
      if (!xmlStrcmp(slave_childs->name, (const xmlChar *)"inputs")) 
       {
        extract_slave_input_desc(slave, slave_childs);        
       }
      if (!xmlStrcmp(slave_childs->name, (const xmlChar *)"holds")) 
        {
         extract_slave_holds (slave, slave_childs);        
        }
      if (!xmlStrcmp(slave_childs->name, (const xmlChar *)"regs")) 
        {
         extract_slave_input_regs(slave, slave_childs);        
        }
    }
  }
}

void extract_master_nodes_content(Common_node_t **master_content_node, int *number, xmlNode *master_node)
{
  for (xmlNode *input_desc = master_node->children; input_desc; input_desc = input_desc->next)
  {
    if (input_desc->type == XML_ELEMENT_NODE) {
      master_address_t *object = malloc(sizeof(struct master_address));
      if (object == NULL)
      {
        printf("failed to allocate for master address object.\n");
        exit(EXIT_FAILURE);
      }

      char value[20] = {'\0'};
      char *endptr;
      strcpy(value, (char *)xmlNodeGetContent(input_desc));
      object->addr = strtoul(value, &endptr, 16);
      add_common_node_to_linklist(master_content_node, (void *)object);
      (*number)++;
    }
  }
}

static void extract_master_coils(master_t *master, xmlNode *master_child)
{
  master->coils_list.number = 0;
  master->coils_list.data_nodes = NULL;
  extract_master_nodes_content(&master->coils_list.data_nodes, &master->coils_list.number, master_child);
}

static void extract_master_inputs(master_t *master, xmlNode *master_child)
{
  master->input_dsc_list.number = 0;
  master->input_dsc_list.data_nodes = NULL;
  extract_master_nodes_content(&master->input_dsc_list.data_nodes,
                               &master->input_dsc_list.number, master_child);
}

static void extract_master_holds(master_t *master, xmlNode *master_child)
{
  master->hold_regs_list.number = 0;
  master->hold_regs_list.data_nodes = NULL;
  extract_master_nodes_content(&master->hold_regs_list.data_nodes, &master->hold_regs_list.number, master_child);
}

static void extract_master_regs(master_t *master, xmlNode *master_child) {
  master->input_regs_list.number = 0;
  master->input_regs_list.data_nodes = NULL;
  extract_master_nodes_content(&master->input_regs_list.data_nodes, &master->input_regs_list.number, master_child);
}

static void traverese_master(xmlNode *master_node, database_t *db)
{
  master_t *master = malloc(sizeof(struct master));

  if (master == NULL)
  {
    printf("failed to allocate for master node./n");
    exit(EXIT_FAILURE);
  }
  
  for (xmlNode *master_child = master_node->children; master_child; master_child = master_child->next)
  {
    if (master_child->type == XML_ELEMENT_NODE)
    {
      if (!xmlStrcmp(master_child->name, (const xmlChar *)"coils"))
      {
        extract_master_coils(master, master_child);
      }
      if (!xmlStrcmp(master_child->name, (const xmlChar *)"inputs")) {
        extract_master_inputs(master, master_child);
      }
      else if (!xmlStrcmp(master_child->name, (const xmlChar *)"holds"))
      {
        extract_master_holds(master, master_child);
      }
      else if (!xmlStrcmp(master_child->name, (const xmlChar *)"regs")) {
        extract_master_regs(master, master_child);
      }
    }
  }
  add_common_node_to_linklist(&db->master_node, (void *)master);
}

static void traverese_slaves(xmlNode *slaves_nodes, database_t *db)
{
  for (xmlNode *slave_node = slaves_nodes->children; slave_node; slave_node = slave_node->next)
  {
    if (slave_node->type == XML_ELEMENT_NODE)
    {
      slave_t *slave = malloc(sizeof(struct slave));
      if (slave == NULL)
      {
        printf("failed to allocate for slave node.\n");
        exit(EXIT_FAILURE);
      }
      getXMLValue_ul(slave_node, "slave_id", "", &slave->slave_id);
      getXMLValue_ul(slave_node, "connection", "port", (int *)&slave->cfg.port);
      getXMLValue(slave_node, "connection", "ip", slave->ip);
      slave->cfg.priority = 15;
      slave->cfg.stack_size = 2048;
      extract_slave_content(slave, slave_node);
      add_common_node_to_linklist(&db->slave_list, (void *)slave);
      db->slaves_number++;
    }
  }
}

static void load_and_parse_database(database_t *db)
{
  xmlNodePtr root = get_root();  
  
  for (xmlNode *slaves_nodes = root->children; slaves_nodes; slaves_nodes = slaves_nodes->next)
  {
    if (slaves_nodes->type == XML_ELEMENT_NODE)
    {
      if (!strcmp((const char *)slaves_nodes->name, "slaves"))
      {
        traverese_slaves(slaves_nodes, db);
      }
      else if (!strcmp((const char *)slaves_nodes->name, "master"))
      {
        traverese_master(slaves_nodes, db);
      }
    }
  }
  xmlFreeDoc(document);
  xmlCleanupParser();  
}

database_t *load_database() 
{
  database_t *db = calloc(1, sizeof(struct database));
  
  if (db == NULL) 
  {
    printf("failed to allocate memory for database object. program exits.\n");
    exit(EXIT_FAILURE);
  }
  load_and_parse_database(db);
  return db;
}

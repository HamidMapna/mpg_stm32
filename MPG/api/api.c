#include "app_commons.h"
//#include "microhttpd_api.h"
#include "mb_tcp_master.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define APP_MAIN_SLEEPTIME_US          5000 * 1000

#define APP_MAIN_SLEEPTIME_US          5000 * 1000
#define APP_SNMP_THREAD_PRIORITY       1
#define APP_SNMP_THREAD_STACKSIZE      256 * 1024 /* bytes */
#define APP_ETH_THREAD_PRIORITY        10
#define APP_ETH_THREAD_STACKSIZE       4096 /* bytes */
#define APP_BG_WORKER_THREAD_PRIORITY  5
#define APP_BG_WORKER_THREAD_STACKSIZE 4096 /* bytes */

void api_init()
{
	app_data_t* mpg_app = NULL;
	pnet_cfg_t pnet_cfg = { 0 };
	app_utils_netif_namelist_t netif_name_list;
	pnet_if_cfg_t netif_cfg = { 0 };
	uint16_t number_of_ports = 1;
	/* Enable line buffering for printouts, especially when logging to
	   the journal (which is default when running as a systemd job) */
	setvbuf(stdout, NULL, _IOLBF, 0);
	/* Prepare configuration */
	app_pnet_cfg_init_default(&pnet_cfg);
	strcpy(pnet_cfg.station_name, "rt-labs-dev");
	int ret = app_utils_pnet_cfg_init_netifs(
		"eth0",
		&netif_name_list,
		&number_of_ports,
		&netif_cfg);
	if (ret != 0)
	{
		exit(EXIT_FAILURE);
	}
	pnet_cfg.if_cfg = netif_cfg;
	pnet_cfg.num_physical_ports = number_of_ports;

	/* Operating system specific settings */
	//pnet_cfg.pnal_cfg.snmp_thread.prio = APP_SNMP_THREAD_PRIORITY;
	//pnet_cfg.pnal_cfg.snmp_thread.stack_size = APP_SNMP_THREAD_STACKSIZE;
	//pnet_cfg.pnal_cfg.eth_recv_thread.prio = APP_ETH_THREAD_PRIORITY;
	//pnet_cfg.pnal_cfg.eth_recv_thread.stack_size = APP_ETH_THREAD_STACKSIZE;
	pnet_cfg.pnal_cfg.bg_worker_thread.prio = APP_BG_WORKER_THREAD_PRIORITY;
	pnet_cfg.pnal_cfg.bg_worker_thread.stack_size = APP_BG_WORKER_THREAD_STACKSIZE;

	/* Initialise stack and application */
	mpg_app = app_init(&pnet_cfg);
	if (mpg_app == NULL)
	{
		printf("Failed to initialize P-Net.\n");
		printf("Do you have enough Ethernet interface permission?\n");
		printf("Aborting application\n");
		exit(EXIT_FAILURE);
	}
	/* Start main loop */
	if (app_start(mpg_app, RUN_IN_SEPARATE_THREAD) != 0)
	{
		printf("Failed to start\n");
		printf("Aborting application\n");
		exit(EXIT_FAILURE);
	}
	
	//create_daemon_handler(HTTP);
	//create_daemon_handler(HTTPS);
	//launch modbus tcp master&slave//
	//init_modbus_tcp();
	
	for (;;)
	{
		os_usleep(APP_MAIN_SLEEPTIME_US);
	}
}

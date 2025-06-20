cmake_minimum_required(VERSION 3.14)
project(pf_src_lib)

include_directories(${ALL_INCLUDES_DIR} ${OSAL_INCLUDE_DIR} ${MPG_SOURCE_DIR}/profinet-dev/src/ports/STM32Cube)

add_library (${PROJECT_NAME}  STATIC
   device/pnet_api.c
   device/pf_block_reader.c
   device/pf_block_writer.c
   device/pf_fspm.c
   device/pf_diag.c
   device/pf_cmdev.c
   device/pf_cmdmc.c
   device/pf_cmina.c
   device/pf_cmio.c
   device/pf_cmpbe.c
   device/pf_cmrdr.c
   device/pf_cmrpc.c
   device/pf_cmrpc_epm.c
   device/pf_cmrpc_helpers.c
   device/pf_cmrs.c
   device/pf_cmsm.c
   device/pf_cmsu.c
   device/pf_cmwrr.c
   device/pf_pdport.c
   device/pf_port.c
   device/pf_plugsm.c
   common/pf_alarm.c
   common/pf_bg_worker.c
   common/pf_cpm.c
   common/pf_cpm_driver_sw.c
   common/pf_dcp.c
   common/pf_ppm.c
   common/pf_ppm_driver_sw.c
   common/pf_ptcp.c
   common/pf_scheduler.c
   common/pf_eth.c
   common/pf_file.c
   common/pf_lldp.c
   $<$<BOOL:${PNET_OPTION_SNMP}>:${mpg_SOURCE_DIR}/src/common/pf_snmp.c>
   common/pf_udp.c
   #ports/linux/pnal_filetools.c
   ports/STM32Cube/pnal_eth.c
   ports/STM32Cube/pnal.c   
   ports/STM32Cube/pnal_udp.c  
   )
   
###############################
  
   #target_link_directories(${PROJECT_NAME} PUBLIC ${OSAL_BINARY_DIR}/)
   #target_link_libraries(${PROJECT_NAME} PRIVATE libosal.a)
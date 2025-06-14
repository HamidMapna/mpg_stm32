cmake_minimum_required(VERSION 3.14)
project(PF_DEV)

set(PF_Binary_Include_Dir ${PF_DEV_BINARY_DIR}/include)
set(PF_Main_Include_Dir ${PF_DEV_SOURCE_DIR}/include)
set(PF_Src_Dir ${PF_DEV_SOURCE_DIR}/src)
set(PF_Src_Common_Dir ${PF_DEV_SOURCE_DIR}/src/common)
set(PF_Src_Device_Dir ${PF_DEV_SOURCE_DIR}/src/device)
#set(Libs_Dir ${MPG_SOURCE_DIR}/libs)

option (PNET_OPTION_FAST_STARTUP "" ON)
option (PNET_OPTION_PARAMETER_SERVER "" ON)
option (PNET_OPTION_IR "" ON)
option (PNET_OPTION_SR "" ON)
option (PNET_OPTION_REDUNDANCY "" ON)
option (PNET_OPTION_AR_VENDOR_BLOCKS "" ON)
option (PNET_OPTION_RS "" ON)
option (PNET_OPTION_MC_CR "" ON)
option (PNET_OPTION_SRL "" OFF)
option (PNET_OPTION_SNMP "" OFF)
option (PNET_USE_ATOMICS "Enable use of atomic operations (stdatomic.h)" OFF)


set(PNET_MAX_AR                 2
  CACHE STRING "Number of connections. Must be > 0. 'Automated RT Tester' uses 2, but only 1 connection AR is supported.")
set(PNET_MAX_API                1
  CACHE STRING "Number of Application Processes. Must be > 0")
set(PNET_MAX_CR                 2
  CACHE STRING "Per AR. At least 2 (1 input and 1 output). If unsure, use 2.")
set(PNET_MAX_SLOTS            5
  CACHE STRING "Per API. Should be > 1 to allow at least one I/O module")
set(PNET_MAX_SUBSLOTS         3
  CACHE STRING "Per slot (DAP requires 2 + PNET_MAX_PHYSICAL_PORTS)")
set(PNET_MAX_DFP_IOCR           2
  CACHE STRING "Allowed values are 0 (zero) or 2")
set(PNET_MAX_PHYSICAL_PORTS          1
  CACHE STRING "Max number of physical ports")
set(PNET_MAX_LOG_BOOK_ENTRIES   16
  CACHE STRING "")
set(PNET_MAX_ALARMS             6
  CACHE STRING "Per AR and queue. One queue for high and one for low priority alarms. 'Automated RT Tester' uses 6.")
set(PNET_MAX_ALARM_PAYLOAD_DATA_SIZE 28
  CACHE STRING "Min 24 or PNET_MAX_DIAG_MANUF_DATA_SIZE + 12. Max is 1408")
set(PNET_MAX_DIAG_ITEMS         200
  CACHE STRING "Total, per device. Max is 65534 items")
set(PNET_MAX_DIAG_MANUF_DATA_SIZE 16
  CACHE STRING "Min 5 for tests. Max is 1396")
set(PNET_MAX_MC_CR              1
  CACHE STRING "Per AR")
set(PNET_MAX_AR_VENDOR_BLOCKS   1
  CACHE STRING "Must be > 0")
set(PNET_MAX_AR_VENDOR_BLOCK_DATA_LENGTH 512
  CACHE STRING "")
set(PNET_MAX_MAN_SPECIFIC_FAST_STARTUP_DATA_LENGTH 0
  CACHE STRING "or 512 (bytes")
set(PNET_MAX_SESSION_BUFFER_SIZE 4500
  CACHE STRING "Max fragmented RPC request/response length. Max value 65535")
set(PNET_MAX_DIRECTORYPATH_SIZE 240
  CACHE STRING "Max size of directory path, including termination")
set(PNET_MAX_FILENAME_SIZE 30
  CACHE STRING "Max size of filename, including termination")
set(PNET_MAX_PORT_DESCRIPTION_SIZE 60
  CACHE STRING "Max size of port description, including termination")

set(LOG_STATE_VALUES "ON;OFF")
set(LOG_LEVEL_VALUES "DEBUG;INFO;WARNING;ERROR;FATAL")

set(LOG_LEVEL FATAL CACHE STRING "default log level")
set_property(CACHE LOG_LEVEL PROPERTY STRINGS ${LOG_LEVEL_VALUES})

set(PF_ETH_LOG ON CACHE STRING "pf_eth log")
set_property(CACHE PF_ETH_LOG PROPERTY STRINGS ${LOG_STATE_VALUES})

set(PF_LLDP_LOG ON CACHE STRING "pf_lldp log")
set_property(CACHE PF_LLDP_LOG PROPERTY STRINGS ${LOG_STATE_VALUES})

set(PF_SNMP_LOG ON CACHE STRING "pf_snmp log")
set_property(CACHE PF_SNMP_LOG PROPERTY STRINGS ${LOG_STATE_VALUES})

set(PF_CPM_LOG ON CACHE STRING "pf_cpm log")
set_property(CACHE PF_CPM_LOG PROPERTY STRINGS ${LOG_STATE_VALUES})

set(PF_PPM_LOG ON CACHE STRING "pf_ppm log")
set_property(CACHE PF_PPM_LOG PROPERTY STRINGS ${LOG_STATE_VALUES})

set(PF_DCP_LOG ON CACHE STRING "pf_dcp log")
set_property(CACHE PF_DCP_LOG PROPERTY STRINGS ${LOG_STATE_VALUES})

set(PF_RPC_LOG ON CACHE STRING "pf_rpc log")
set_property(CACHE PF_RPC_LOG PROPERTY STRINGS ${LOG_STATE_VALUES})

set(PF_ALARM_LOG ON CACHE STRING "pf_alarm log")
set_property(CACHE PF_ALARM_LOG PROPERTY STRINGS ${LOG_STATE_VALUES})

set(PF_AL_BUF_LOG ON CACHE STRING "pf_alarm_buffer log")
set_property(CACHE PF_AL_BUF_LOG PROPERTY STRINGS ${LOG_STATE_VALUES})

set(PF_PNAL_LOG ON CACHE STRING "PNAL log")
set_property(CACHE PF_PNAL_LOG PROPERTY STRINGS ${LOG_STATE_VALUES})

set(PNET_LOG ON CACHE STRING "PNET log")
set_property(CACHE PNET_LOG PROPERTY STRINGS ${LOG_STATE_VALUES})

#Generate version numbers
configure_file (
    pnet_version.h.in
	${PF_Binary_Include_Dir}/pnet_version.h
 )

# Generate config options
configure_file (
  options.h.in
  ${PF_Binary_Include_Dir}/options.h
  )

configure_file (
  pnet_options.h.in
  ${PF_Binary_Include_Dir}/pnet_options.h
  )

  include_directories(
	${PF_Main_Include_Dir}/
	${PF_Src_Common_Dir}/
	${PF_Src_Device_Dir}/
    ${PF_Binary_Include_Dir}/
	${PF_Src_Dir}/	
  )

  #add_subdirectory(${Libs_Dir}/osal/ osal)
  add_subdirectory(${PF_Src_Dir}/ pf_src_lib)
  
cmake_minimum_required(VERSION 3.14)
project(commonLib)

set(PF_Ports_Linux_Dir ${PF_DEV_SOURCE_DIR}/src/ports/linux)
add_subdirectory(${PF_Ports_Linux_Dir}/ portsLib)

set(COMM_SRC  
pf_alarm.c
pf_bg_worker.c
pf_cpm.c
pf_cpm_driver_sw.c
pf_dcp.c
pf_eth.c
pf_file.c
pf_lldp.c
pf_ppm.c
pf_ppm_driver_sw.c
pf_ptcp.c
pf_scheduler.c
pf_udp.c
)
set(COMM_HEADER
pf_alarm.h
pf_bg_worker.h
pf_cpm.h
pf_cpm_driver_sw.h
pf_dcp.h
pf_eth.h
pf_file.h
pf_lldp.h
pf_ppm.h
pf_ppm_driver_sw.h
pf_ptcp.h
pf_scheduler.h
pf_udp.h
)
add_library(${PROJECT_NAME} SHARED ${COMM_HEADER} ${COMM_SRC})
target_link_libraries(${PROJECT_NAME} PUBLIC portsLib osal)
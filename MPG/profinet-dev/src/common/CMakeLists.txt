cmake_minimum_required(VERSION 3.14)
project(commonLib)

add_subdirectory(${PF_DEV_SOURCE_DIR}/src/ports/STM32Cube portsLib)

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

include_directories(${ALL_INCLUDES_DIR} ${PF_Comm_Dir} ${PF_dev_Dir} ${OSAL_SRC_FRTOS_SYS_DIR} ${OSAL_INCLUDE_DIR})
add_library(${PROJECT_NAME} STATIC ${COMM_HEADER} ${COMM_SRC})
target_link_libraries(${PROJECT_NAME} PUBLIC portsLib)

target_compile_options(${PROJECT_NAME} PRIVATE ${MCPU} ${MFLOAT_ABI} -mthumb ${MFPU})
target_link_options(${PROJECT_NAME} PRIVATE -T${LINKER_SCRIPT} ${MCPU} ${MFLOAT_ABI} -mthumb ${MFPU})
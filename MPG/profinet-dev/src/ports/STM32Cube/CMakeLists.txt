cmake_minimum_required(VERSION 3.14)
project(portsLib)

set(SRC_FILES
 pnal.c
 pnal_eth.c 
 pnal_udp.c
)
set(HEADER_FILES
 pnal_config.h
 pnal_sys.h
)
include_directories(${OSAL_INCLUDE_DIR} ${OSAL_SRC_FRTOS_SYS_DIR})
add_library(${PROJECT_NAME} STATIC ${HEADER_FILES} ${SRC_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${ALL_INCLUDES_DIR})

target_compile_options(${PROJECT_NAME} PRIVATE ${MCPU} ${MFLOAT_ABI} -mthumb ${MFPU})
target_link_options(${PROJECT_NAME} PRIVATE -T${LINKER_SCRIPT} ${MCPU} ${MFLOAT_ABI} -mthumb ${MFPU})
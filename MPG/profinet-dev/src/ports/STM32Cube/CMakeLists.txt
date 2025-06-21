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
include_directories(${OSAL_INCLUDE_DIR})
add_library(${PROJECT_NAME} STATIC ${HEADER_FILES} ${SRC_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${ALL_INCLUDES_DIR})
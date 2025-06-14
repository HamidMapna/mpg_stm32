cmake_minimum_required(VERSION 3.14)
project(portsLib)

set(SRC_FILES
 pnal.c
 pnal_eth.c
 pnal_filetools.c
 pnal_udp.c
)
set(HEADER_FILES
 pnal_config.h
 pnal_filetools.h
 pnal_sys.h
)
add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SRC_FILES})
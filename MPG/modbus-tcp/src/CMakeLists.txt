cmake_minimum_required(VERSION 3.14)
project(mb_srcLib)
#link_directories(/usr/local/lib/arm-linux-gnueabihf/ /usr/lib/arm-linux-gnueabihf/ ${MHD_LIBRARY_DIR}/)

set(MB_TCP_INCLUDE_DIR ${MB_TCP_SOURCE_DIR}/include)
set(LIBXML2_INCLUDE_DIR /usr/include/libxml2)
set(DATABASE_FILE /usr/local/share/xml/db.xml)
set(Api_Core_Dir ${MB_TCP_SOURCE_DIR}/api/Core)

set(LOG_STATE_VALUES "ON;OFF")
set(MB_TCP_LOG ON CACHE STRING "tcp log")
set_property(CACHE MB_TCP_LOG PROPERTY STRINGS ${LOG_STATE_VALUES})
add_definitions(-DMB_TCP_LOG)
# Pass database path to the code as a preprocessor definition
add_definitions(-DDATABASE_FILE="${DATABASE_FILE}")
add_subdirectory(${mb_srcLib_SOURCE_DIR}/ports/STM32Cube/ mb_stm32_srcLib)

include_directories(
${MB_TCP_BINARY_DIR}/src/
${MB_TCP_INCLUDE_DIR}/
${LIBXML2_INCLUDE_DIR}/
${mb_srcLib_SOURCE_DIR}/ports/STM32Cube/
${Api_Core_Dir}/
${ALL_INCLUDES_DIR}
#${OSAL_SOURCE_DIR}/include/
#${OSAL_SOURCE_DIR}/src/linux/
)

set(mbus_SRC 
  ${MB_TCP_INCLUDE_DIR}/mb_master.h
  ${MB_TCP_INCLUDE_DIR}/mb_slave.h
  ${MB_TCP_INCLUDE_DIR}/mb_transport.h
  ${MB_TCP_INCLUDE_DIR}/mb_tcp.h
  ${MB_TCP_INCLUDE_DIR}/mb_error.h
  ${MB_TCP_SOURCE_DIR}/db/xml.h
  #${MB_TCP_SOURCE_DIR}/db/xml.c  
  #${MB_TCP_SOURCE_DIR}/main/mb_tcp_slave.c  
  #${MB_TCP_SOURCE_DIR}/main/mb_tcp_master.c 
  mbus.c
  mb_slave.c
  mb_transport.c
  mb_tcp.c
  mb_crc.c
  mb_crc.h
  mb_pdu.h
  )
  
add_library(${PROJECT_NAME}  STATIC ${mbus_SRC})
target_link_libraries(${PROJECT_NAME} PUBLIC mb_stm32_srcLib xml2)
cmake_minimum_required(VERSION 3.14)
project(mb_linux_srcLib)

set(MBUS_SOURCE_DIR ${MPG_SOURCE_DIR}/modbus-tcp/)

include_directories(
	${MBUS_SOURCE_DIR}/src/ports/linux/
	${MB_TCP_SOURCE_DIR}/include/
	${OSAL_SOURCE_DIR}/include/
	${OSAL_SOURCE_DIR}/src/linux/
)
file(GLOB_RECURSE mb_linux_SRC  "mbal_tcp.c" "mbal_sys.h")
add_library(${PROJECT_NAME}  STATIC ${mb_linux_SRC})
target_include_directories(${PROJECT_NAME} PUBLIC ${MBUS_SOURCE_DIR}/src/)

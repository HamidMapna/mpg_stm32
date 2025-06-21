cmake_minimum_required(VERSION 3.14)
project(mb_stm32_srcLib)

set(MBUS_SOURCE_DIR ${MPG_SOURCE_DIR}/modbus-tcp/)

include_directories(
	${ALL_INCLUDES_DIR}
	${MBUS_SOURCE_DIR}/src/ports/STM32Cube/
	${MB_TCP_SOURCE_DIR}/include/
	${OSAL_SOURCE_DIR}/include/
	${OSAL_SOURCE_DIR}/src/freertos/sys/
)
file(GLOB_RECURSE mb_stm32_SRC  "mbal_tcp.c" "mbal_sys.h")
add_library(${PROJECT_NAME}  STATIC ${mb_stm32_SRC})
target_include_directories(${PROJECT_NAME} PUBLIC ${MBUS_SOURCE_DIR}/src/)

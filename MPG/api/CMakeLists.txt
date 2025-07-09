cmake_minimum_required(VERSION 3.20)
project(API)

set(Api_Core_Dir ${API_SOURCE_DIR}/Core)
set(Mb_Tcp_Dir ${MPG_SOURCE_DIR}/modbus-tcp)

set(Mb_Includes
	${PF_DEV_SOURCE_DIR}/include/	
	${PF_DEV_SOURCE_DIR}/src/
	${PF_DEV_SOURCE_DIR}/src/ports/STM32Cube/		
	${PF_DEV_BINARY_DIR}/include/
	${Api_Core_Dir}/
	${Mb_Tcp_Dir}/include/
	${Mb_Tcp_Dir}/db/
	${Mb_Tcp_Dir}/main/
	${CMAKE_CURRENT_SOURCE_DIR}
)

include_directories(${Mb_Includes} ${ALL_INCLUDES_DIR} ${OSAL_INCLUDE_DIR} ${OSAL_SRC_FRTOS_SYS_DIR})

add_subdirectory(${Api_Core_Dir} api_core)
add_subdirectory(${Mb_Tcp_Dir}/main Mb_Tcp_Main)
add_subdirectory(${MPG_SOURCE_DIR}/osal/ OSAL)

add_library(${PROJECT_NAME} STATIC api.c)
target_link_libraries(${PROJECT_NAME} PUBLIC OSAL api_core Mb_Tcp_Main)

target_compile_options(${PROJECT_NAME} PRIVATE ${MCPU} ${MFLOAT_ABI} -mthumb ${MFPU})
target_link_options(${PROJECT_NAME} PRIVATE -T${LINKER_SCRIPT} ${MCPU} ${MFLOAT_ABI} -mthumb ${MFPU})


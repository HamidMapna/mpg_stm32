cmake_minimum_required(VERSION 3.14)
project(MPG)

set (PROJECT_DEFINES
	# LIST COMPILER DEFINITIONS HERE
	STM32F407xx
	USE_HAL_DRIVER
    )

set(Api_Dir		${MPG_SOURCE_DIR}/api)
set(Pf_Dev_Dir	${MPG_SOURCE_DIR}/profinet-dev)
set(Mb_Tcp_Dir	${MPG_SOURCE_DIR}/modbus-tcp)

add_compile_definitions (${PROJECT_DEFINES})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/osal/ osal)
add_subdirectory(${Pf_Dev_Dir}/ PF_DEV)
#add_subdirectory(${Mb_Tcp_Dir}/ MB_TCP)
#add_subdirectory(${Api_Dir}/ API)


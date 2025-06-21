cmake_minimum_required(VERSION 3.20)
project(MPG)

set (PROJECT_DEFINES
	# LIST COMPILER DEFINITIONS HERE
	STM32F407xx
	USE_HAL_DRIVER
    )

set(Api_Dir		${MPG_SOURCE_DIR}/api)
set(Pf_Dev_Dir	${MPG_SOURCE_DIR}/profinet-dev)
set(Mb_Tcp_Dir	${MPG_SOURCE_DIR}/modbus-tcp)
set(OSAL_SRC_FRTOS_SYS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/osal/src/freertos)
set(OSAL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/osal/include)

add_compile_definitions (${PROJECT_DEFINES})

set(ALL_INCLUDES_DIR ${MDD_LWIP_SRC_INCLUDE} ${MDD_LWIP_SYS} ${LWIP_TARGET} ${Core_Inc} ${DRV_HAL_Inc}
 ${DRV_CMSIS_DEV_ST_INCLUDE} ${DRV_CMSIS_INCLUDE} ${OSAL_SRC_FRTOS_SYS_DIR} ${OSAL_INCLUDE_DIR} ${MDD_FRTOS_SRC_INCLUDE}
 ${MDD_FRTOS_SRC_GCC_ARM_CM4} ${MDD_FRTOS_SRC_CMSIS_V2})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/osal/ osal)
add_subdirectory(${Pf_Dev_Dir}/ PF_DEV)
#add_subdirectory(${Mb_Tcp_Dir}/ MB_TCP)
add_subdirectory(${Api_Dir}/ API)


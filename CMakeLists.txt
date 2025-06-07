#############################################################################################################################
# file:  CMakeLists.txt
# brief: Template "CMakeLists.txt" for building of executables and static libraries.
#
# usage: Edit "VARIABLES"-section to suit project requirements.
#        For debug build:
#          cmake -DCMAKE_TOOLCHAIN_FILE=cubeide-gcc.cmake  -S ./ -B Debug -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug
#          make -C Debug VERBOSE=1 -j
#        For release build:
#          cmake -DCMAKE_TOOLCHAIN_FILE=cubeide-gcc.cmake  -S ./ -B Release -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
#          make -C Release VERBOSE=1 -j
#############################################################################################################################
cmake_minimum_required(VERSION 3.20)

#######################################################################
set(CMAKE_TOOLCHAIN_PATH "E:/Programs/Cube/STM32CubeIDE_1.17.0/STM32CubeIDE/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.12.3.rel1.win32_1.1.0.202410251130/tools/bin/")
set(STLINK_CLI_PATH "C:/Program Files (x86)/STMicroelectronics/STM32 ST-LINK Utility/ST-LINK Utility/ST-LINK_CLI.exe")
set(TOOLCHAIN_PREFIX arm-none-eabi-)
###################### CONSTANTS ######################################
set (PROJECT_TYPE_EXECUTABLE          "exe")
set (PROJECT_TYPE_STATIC_LIBRARY      "static-lib")
set (MCPU_CORTEX_M0				      "-mcpu=cortex-m0")
set (MCPU_CORTEX_M0PLUS				  "-mcpu=cortex-m0plus")
set (MCPU_CORTEX_M3				      "-mcpu=cortex-m3")
set (MCPU_CORTEX_M4				      "-mcpu=cortex-m4")
set (MCPU_CORTEX_M7				      "-mcpu=cortex-m7")
set (MCPU_CORTEX_M33				  "-mcpu=cortex-m33")
set (MCPU_CORTEX_M55				  "-mcpu=cortex-m55")
set (MCPU_CORTEX_M85				  "-mcpu=cortex-m85")
set (MFPU_FPV4_SP_D16                 "-mfpu=fpv4-sp-d16")
set (MFPU_FPV5_D16                    "-mfpu=fpv5-d16")
set (RUNTIME_LIBRARY_REDUCED_C        "--specs=nano.specs")
set (RUNTIME_LIBRARY_STD_C            "")
set (RUNTIME_LIBRARY_SYSCALLS_MINIMAL "--specs=nosys.specs")
set (RUNTIME_LIBRARY_SYSCALLS_NONE    "")
set (MFLOAT_ABI_SOFTWARE              "-mfloat-abi=soft")
set (MFLOAT_ABI_HARDWARE              "-mfloat-abi=hard")
set (MFLOAT_ABI_MIX                   "-mfloat-abi=softfp")
#######################################################################

###################### VARIABLES ######################################
set (PROJECT_NAME             "mpg_stm32f4_cmake2")
set (PROJECT_TYPE             "exe")
set (LINKER_SCRIPT            "../STM32F407VETX_FLASH.ld")
set (MCPU                     "-mcpu=Cortex-M4")
set (MFPU                 "-mfpu=fpv4-sp-d16")
set (MFLOAT_ABI               "-mfloat-abi=hard")
set (RUNTIME_LIBRARY          "--specs=nano.specs")
set (RUNTIME_LIBRARY_SYSCALLS "--specs=nosys.specs")

############Set Directories############################################
set(CORE_DIR ${CMAKE_SOURCE_DIR}/Core)
set(Core1 ${CORE_DIR}/Inc/)
set(Core2 ${CORE_DIR}/tm_stm32f4_usart/)
set(Src_Dir ${CORE_DIR}/Src/)
set(Startup_Dir ${CORE_DIR}/Startup/)
#######################################################################
set(DRIVERS_DIR ${CMAKE_SOURCE_DIR}/Drivers/)
set(DRIVERS1 ${DRIVERS_DIR}/CMSIS/Include)
set(DRIVERS2 ${DRIVERS_DIR}/CMSIS/Device/ST/STM32F4xx/Include)
set(DRIVERS3 ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Inc/Legacy)
set(DRIVERS4 ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Inc)
set(DRIVERS5 ${DRIVERS_DIR}/BSP/Components/dp83848)

set(LWIP1 ${CMAKE_SOURCE_DIR}/LWIP/App/)
set(LWIP2 ${CMAKE_SOURCE_DIR}/LWIP/Target/)

set(MIDDLEWARE ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party)
set(MIDDWARE1 ${MIDDLEWARE}/LwIP/src/include)
set(MIDDWARE2 ${MIDDLEWARE}/LwIP/system)
set(MIDDWARE3 ${MIDDLEWARE}/LwIP/src/include/netif/ppp)
set(MIDDWARE4 ${MIDDLEWARE}/LwIP/src/include/lwip)
set(MIDDWARE5 ${MIDDLEWARE}/LwIP/src/include/lwip/apps)
set(MIDDWARE6 ${MIDDLEWARE}/LwIP/src/include/lwip/priv)
set(MIDDWARE7 ${MIDDLEWARE}/LwIP/src/include/lwip/prot)
set(MIDDWARE8 ${MIDDLEWARE}/LwIP/src/include/netif)
set(MIDDWARE9 ${MIDDLEWARE}/LwIP/src/include/compat/posix)
set(MIDDWARE10 ${MIDDLEWARE}/LwIP/src/include/compat/posix/arpa)
set(MIDDWARE11 ${MIDDLEWARE}/LwIP/src/include/compat/posix/net)
set(MIDDWARE12 ${MIDDLEWARE}/LwIP/src/include/compat/posix/sys)
set(MIDDWARE13 ${MIDDLEWARE}/LwIP/src/include/compat/stdc)
set(MIDDWARE14 ${MIDDLEWARE}/LwIP/system/arch)
set(MIDDWARE15 ${MIDDLEWARE}/FreeRTOS/Source/include)
set(MIDDWARE16 ${MIDDLEWARE}/FreeRTOS/Source/portable/GCC/ARM_CM4F)
set(MIDDWARE17 ${MIDDLEWARE}/FreeRTOS/Source/CMSIS_RTOS_V2)

################## PROJECT SETUP ######################################
set(DRIVERS_SRC1 ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src)
set (PROJECT_SOURCES
	# LIST SOURCE FILES HERE
		${Startup_Dir}/startup_stm32f407vetx.s
		${Src_Dir}/freertos.c
		${Src_Dir}/main.c
		${Src_Dir}/syscalls.c
		${Src_Dir}/sysmem.c	
		${Src_Dir}/system_stm32f4xx.c
		${Src_Dir}/stm32f4xx_it.c
		${Src_Dir}/stm32f4xx_hal_msp.c	
		${Src_Dir}/stm32f4xx_hal_timebase_tim.c
		${DRIVERS5}/dp83848.c
		${DRIVERS_SRC1}/stm32f4xx_hal.c
		${DRIVERS_SRC1}/stm32f4xx_hal_uart.c
		${DRIVERS_SRC1}/stm32f4xx_hal_tim.c
		${DRIVERS_SRC1}/stm32f4xx_hal_tim_ex.c
		${DRIVERS_SRC1}/stm32f4xx_hal_rcc.c
		${DRIVERS_SRC1}/stm32f4xx_hal_rcc_ex.c
		${DRIVERS_SRC1}/stm32f4xx_hal_pwr.c
		${DRIVERS_SRC1}/stm32f4xx_hal_pwr_ex.c
		${DRIVERS_SRC1}/stm32f4xx_hal_gpio.c
		${DRIVERS_SRC1}/stm32f4xx_hal_flash.c
		${DRIVERS_SRC1}/stm32f4xx_hal_flash_ramfunc.c
		${DRIVERS_SRC1}/stm32f4xx_hal_flash_ex.c
		${DRIVERS_SRC1}/stm32f4xx_hal_exti.c
		${DRIVERS_SRC1}/stm32f4xx_hal_eth.c
		${DRIVERS_SRC1}/stm32f4xx_hal_dma.c
		${DRIVERS_SRC1}/stm32f4xx_hal_dma_ex.c
		${DRIVERS_SRC1}/stm32f4xx_hal_cortex.c
		${LWIP1}/lwip.c
		${LWIP2}/ethernetif.c
		##############
		  ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c
		  ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/event_groups.c
		  ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/list.c
		  ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/croutine.c

		  ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/queue.c
		  ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c
		  ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/tasks.c
		  ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/timers.c

		  ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/api/api_lib.c
		  ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/api/api_msg.c
		  ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/api/err.c
		  ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/api/if_api.c
		  ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/api/netbuf.c
		  ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/api/netdb.c
		  ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/api/netifapi.c
		  ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/api/sockets.c
		  ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/api/tcpip.c
		  ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/def.c
		  ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/init.c
		  ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/ip.c
		  ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/ipv4/dhcp.c
		  ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/ipv4/etharp.c
		  ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/ipv4/icmp.c
		  ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/ipv4/ip4.c
		  ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/ipv4/ip4_addr.c
		  ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/ipv4/ip4_frag.c
		  ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/mem.c
		  ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/memp.c
		  ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/netif.c
		  ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/pbuf.c
		  ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/stats.c
		  ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/tcp.c
		  ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/tcp_in.c
		  ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/tcp_out.c
		  ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/timeouts.c
		  ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/udp.c
		  ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/netif/ethernet.c
		  ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/system/OS/sys_arch.c
  		  ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c
		  ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c
	)


set (PROJECT_DEFINES
	# LIST COMPILER DEFINITIONS HERE
	STM32F407xx
	USE_HAL_DRIVER
    )

set (PROJECT_INCLUDES
	# LIST INCLUDE DIRECTORIES HERE
	${Core1} ${Core2} 
	${DRIVERS1} ${DRIVERS2} ${DRIVERS3} ${DRIVERS4} ${DRIVERS5}
	${LWIP1} ${LWIP2}
	${MIDDWARE1} ${MIDDWARE2} ${MIDDWARE3} ${MIDDWARE4} ${MIDDWARE5} ${MIDDWARE6}
	${MIDDWARE7} ${MIDDWARE8} ${MIDDWARE9} ${MIDDWARE10} ${MIDDWARE11} ${MIDDWARE12}
	${MIDDWARE13} ${MIDDWARE14} ${MIDDWARE15} ${MIDDWARE16} ${MIDDWARE17} 
    )
############ MODIFY ACCORDING TO REQUIREMENTS) ########################
set (CMAKE_C_FLAGS_INIT "${MCPU} -std=gnu11 ${MFPU} ${MFLOAT_ABI} ${RUNTIME_LIBRARY} -mthumb -Wall -Werror")
set (CMAKE_CXX_FLAGS_INIT "${MCPU} -std=gnu11 ${MFPU} ${MFLOAT_ABI} ${RUNTIME_LIBRARY} -mthumb -Wall -Werror")
set (CMAKE_EXE_LINKER_FLAGS_INIT "${CMAKE_C_FLAGS_INIT} -Wl,--gc-sections -specs=nosys.specs")
set(CMAKE_TRY_COMPILER_TARGET_TYPE STATIC_LIBRARY)
set (CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS_INIT}")
set(CPU_FLAGS )

################## PROJECT SETUP ######################################
project(${PROJECT_NAME})
enable_language(ASM)


set(CMAKE_SYSTEM_PROCESSOR cortex-m4)
set(CMAKE_SYSTEM_NAME STM32Cube)
set(BOARD STM32F769I-DISCO)
set(CMAKE_SOURCE_DIR ${CMAKE_SOURCE_DIR})
set(CMAKE_C_COMPILER ${CMAKE_TOOLCHAIN_PATH}/${TOOLCHAIN_PREFIX}gcc.exe)
set(CMAKE_CXX_COMPILER  ${CMAKE_TOOLCHAIN_PATH}/${TOOLCHAIN_PREFIX}g++.exe)
set(CMAKE_ASM_COMPILER ${CMAKE_TOOLCHAIN_PATH}/${TOOLCHAIN_PREFIX}gcc.exe)

if (${PROJECT_TYPE} MATCHES ${PROJECT_TYPE_EXECUTABLE})
  add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
  add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${CMAKE_PROJECT_NAME}>)
elseif (${PROJECT_TYPE} MATCHES ${PROJECT_TYPE_STATIC_LIBRARY})
  add_library(${PROJECT_NAME} ${PROJECT_SOURCES})
endif()

add_compile_definitions (${PROJECT_DEFINES})
target_compile_options(${PROJECT_NAME} PRIVATE ${MCPU} ${MFLOAT_ABI} -mthumb ${MFPU})
target_link_options(${PROJECT_NAME} PRIVATE -T${LINKER_SCRIPT} ${MCPU} ${MFLOAT_ABI} -mthumb ${MFPU})
include_directories (${PROJECT_INCLUDES})

set (CMAKE_EXECUTABLE_SUFFIX ".elf")
set (CMAKE_STATIC_LIBRARY_SUFFIX ".a")
###################################Add subdirectories#####################
#add_subdirectory(Drivers/STM32F4xx_HAL_Driver/Src/ STM_HAL_DRV)
#add_subdirectory(Middlewares/Third_Party/LwIP/ MIDDWARE_MOD)
#add_subdirectory(Middlewares/Third_Party/FreeRTOS/Source/ FREERTOS_MOD)
#add_subdirectory(${CMAKE_SOURCE_DIR}/osal/ OSAL)

#target_link_libraries(${PROJECT_NAME} PUBLIC STM_HAL_DRV MIDDWARE_MOD FREERTOS_MOD)
###################################add custom build command to generate bin and hex file##############
 set(ELF_FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.elf)
 set(HEX_FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.hex)
 set(BIN_FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bin)
############################################################
 add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY} -O ihex ${ELF_FILE} ${HEX_FILE} 
					 COMMAND ${CMAKE_OBJCOPY} -O binary ${ELF_FILE} ${BIN_FILE}
					 COMMENT "Generating HEX and BINfiles" 
)
############################################################
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
					 COMMAND "${STLINK_CLI_PATH}" -c SWD -P ${HEX_FILE} HEX -V -Rst
					 COMMENT  "Flashing HEX file via ST-LINK CLI"
)



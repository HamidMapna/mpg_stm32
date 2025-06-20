cmake_minimum_required(VERSION 3.20)
project(API)

#set(Rest_Dir ${API_SOURCE_DIR}/Controllers/REST)
set(Api_Core_Dir ${API_SOURCE_DIR}/Core)
#set(Mb_Tcp_Dir ${MPG_SOURCE_DIR}/modbus-tcp)
#set(Libs_Dir ${MPG_SOURCE_DIR}/libs)

include_directories(
	${PF_DEV_SOURCE_DIR}/include/	
	${PF_DEV_SOURCE_DIR}/src/
	${PF_DEV_SOURCE_DIR}/src/ports/STM32Cube/		
	${PF_DEV_BINARY_DIR}/include/
	${Api_Core_Dir}/ 
#	${Rest_Dir}/
#	${Mb_Tcp_Dir}/include/
#	${Mb_Tcp_Dir}/db/
#	${Mb_Tcp_Dir}/main/
)

#find_package(Threads REQUIRED)

#add_subdirectory(${Libs_Dir}/microhttpd mhd)
add_subdirectory(${Api_Core_Dir} api_core)
#add_subdirectory(${Mb_Tcp_Dir}/main Mb_Tcp_Main)

#if(MHD_INCLUDE_DIR)
#	include_directories(${MHD_INCLUDE_DIR})
#	get_filename_component(MHD_LIBRARY_DIR ${MHD_LIBRARY} DIRECTORY)
#else()
#	add_dependencies(${PROJECT_NAME} libmicrohttpd)		
#endif()

message("OSAL_BINARY_DIR=${OSAL_BINARY_DIR}")
#add_subdirectory(${Rest_Dir}/ rest)
#link_directories(/usr/local/lib/arm-linux-gnueabihf/ /usr/lib/arm-linux-gnueabihf/ ${MHD_LIBRARY_DIR}/)

#add_executable(${PROJECT_NAME} Api_main.c)
#target_link_libraries(${PROJECT_NAME} PUBLIC api_core rt rest json-c microhttpd osal mb_srcLib Mb_Tcp_Main)

#install(#Give permission 755
#	FILES ${PF_DEV_SOURCE_DIR}/set_network_parameters 
#	DESTINATION ${MPG_SOURCE_DIR}/build/${PROJECT_NAME}/
#	PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
#			   GROUP_READ GROUP_EXECUTE
#			   WORLD_READ WORLD_EXECUTE
#)
#install(FILES ${MB_TCP_SOURCE_DIR}/db/db.xml DESTINATION /usr/local/share/xml/)
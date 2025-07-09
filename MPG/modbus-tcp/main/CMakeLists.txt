cmake_minimum_required(VERSION 3.14)
project(Mb_Tcp_Main)

set(MB_Src_Dir ${MB_TCP_SOURCE_DIR}/src)

add_subdirectory (${MB_Src_Dir}/ mb_srcLib)

include_directories(
	${ALL_INCLUDES_DIR}
	${OSAL_INCLUDE_DIR}
	${OSAL_SRC_FRTOS_SYS_DIR}
)
file(GLOB_RECURSE Tcp_Main_SRC "./*.c" "./*.h")

add_library(${PROJECT_NAME} STATIC ${Tcp_Main_SRC})
target_link_libraries(${PROJECT_NAME} PRIVATE mb_srcLib)
target_link_directories(${PROJECT_NAME} PUBLIC ${OSAL_BINARY_DIR}/)

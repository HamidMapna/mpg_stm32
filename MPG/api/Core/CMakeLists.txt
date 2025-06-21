cmake_minimum_required(VERSION 3.20)
project(api_core)

set(PF_Dev_Dir ${PF_DEV_SOURCE_DIR}/src/device)
message("Mb_Includes=${Mb_Includes}")
include_directories(${ALL_INCLUDES_DIR})

add_subdirectory(${PF_Dev_Dir} deviceLib)

file(GLOB_RECURSE api-core_src "*.c" "*.h")
add_library(${PROJECT_NAME} STATIC ${api-core_src})
target_link_libraries(${PROJECT_NAME} PRIVATE libosal.a commonLib deviceLib)
target_link_directories(${PROJECT_NAME} PUBLIC ${OSAL_BINARY_DIR}/)

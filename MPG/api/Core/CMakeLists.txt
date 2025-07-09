cmake_minimum_required(VERSION 3.20)
project(api_core)

set(PF_Dev_Dir ${PF_DEV_SOURCE_DIR}/src/device)
include_directories(${ALL_INCLUDES_DIR})

add_subdirectory(${PF_Dev_Dir} deviceLib)

file(GLOB_RECURSE api-core_src "*.c" "*.h")
add_library(${PROJECT_NAME} STATIC ${api-core_src})
target_link_libraries(${PROJECT_NAME} PRIVATE commonLib deviceLib)
target_link_directories(${PROJECT_NAME} PUBLIC ${OSAL_BINARY_DIR}/)

target_compile_options(${PROJECT_NAME} PRIVATE ${MCPU} ${MFLOAT_ABI} -mthumb ${MFPU})
target_link_options(${PROJECT_NAME} PRIVATE -T${LINKER_SCRIPT} ${MCPU} ${MFLOAT_ABI} -mthumb ${MFPU})
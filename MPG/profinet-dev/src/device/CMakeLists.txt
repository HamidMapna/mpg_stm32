cmake_minimum_required(VERSION 3.14)
project(deviceLib)

set(PF_Comm_Dir ${PF_DEV_SOURCE_DIR}/src/common)
set(PF_dev_Dir ${PF_DEV_SOURCE_DIR}/src/device)

include_directories(${ALL_INCLUDES_DIR} ${PF_Comm_Dir} ${PF_dev_Dir} ${OSAL_INCLUDE_DIR} ${OSAL_SRC_FRTOS_SYS_DIR})
add_subdirectory(${PF_Comm_Dir} commonLib)

file(GLOB_RECURSE SRC "./*.c" "./*.h")
add_library(${PROJECT_NAME} STATIC ${SRC})
target_link_libraries(${PROJECT_NAME} PUBLIC commonLib)

target_compile_options(${PROJECT_NAME} PRIVATE ${MCPU} ${MFLOAT_ABI} -mthumb ${MFPU})
target_link_options(${PROJECT_NAME} PRIVATE -T${LINKER_SCRIPT} ${MCPU} ${MFLOAT_ABI} -mthumb ${MFPU})
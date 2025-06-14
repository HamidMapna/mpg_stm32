cmake_minimum_required(VERSION 3.14)
project(deviceLib)

set(PF_Comm_Dir ${PF_DEV_SOURCE_DIR}/src/common)
add_subdirectory(${PF_Comm_Dir} commonLib)

file(GLOB_RECURSE SRC "./*.c" "./*.h")
add_library(${PROJECT_NAME} SHARED ${SRC})
target_link_libraries(${PROJECT_NAME} PUBLIC commonLib)
cmake_minimum_required(VERSION 3.20)
project(OSAL)

set(OSAL_SRC_FILES ${OSAL_SRC_FRTOS_SYS_DIR}/osal.c ${OSAL_SRC_FRTOS_SYS_DIR}/osal_log.c)
set(OSAL_SPECIAL_INCLUDE_DIR  ${OSAL_SRC_FRTOS_SYS_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include ${MDD_FRTOS_SRC_INCLUDE} ${Core_Inc} ${MDD_FRTOS_SRC_GCC_ARM_CM4})


add_library(${PROJECT_NAME} STATIC ${OSAL_SRC_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${OSAL_SPECIAL_INCLUDE_DIR})

target_compile_options(${PROJECT_NAME} PUBLIC ${MCPU} ${MFLOAT_ABI} -mthumb ${MFPU})
target_link_options(${PROJECT_NAME} PUBLIC -T${LINKER_SCRIPT} ${MCPU} ${MFLOAT_ABI} -mthumb ${MFPU})

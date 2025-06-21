cmake_minimum_required (VERSION 3.14)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/tools")
project (MB_TCP VERSION 0.1.0)

include(AddOsal)
include(GenerateExportHeader)
include(CMakeDependentOption)

set(LIBXML2_INCLUDE_DIR /usr/include/libxml2)
set(DATABASE_FILE /usr/local/share/xml/db.xml)
set(XML_DIR ${MB_TCP_SOURCE_DIR}/db)
set(Libs_Dir ${MPG_SOURCE_DIR}/libs)
set(LOG_STATE_VALUES "ON;OFF")
set(LOG_LEVEL_VALUES "DEBUG;INFO;WARNING;ERROR")

set(LOG_LEVEL ERROR CACHE STRING "default log level")
set_property(CACHE LOG_LEVEL PROPERTY STRINGS ${LOG_LEVEL_VALUES})

set(MB_TCP_LOG ON CACHE STRING "tcp log")
set_property(CACHE MB_TCP_LOG PROPERTY STRINGS ${LOG_STATE_VALUES})

set(MBUS_TIMEOUT "100"
  CACHE STRING "timeout in ms for something")

# Pass database path to the code as a preprocessor definition
add_definitions(-DDATABASE_FILE="${DATABASE_FILE}")
# Generate version numbers
configure_file (
  version.h.in
  ${MB_TCP_BINARY_DIR}/src/version.h
  )

# Generate config options
configure_file (
  options.h.in
  ${MB_TCP_BINARY_DIR}/src/options.h
  )

include_directories(${MB_TCP_SOURCE_DIR}/db/)
 
#add_subdirectory(${Libs_Dir}/xml2/ xml2)
#add_subdirectory (${MB_Src_Dir}/ mb_srcLib)

#install(FILES ${XML_DIR}/db.xml DESTINATION /usr/local/share/xml/)

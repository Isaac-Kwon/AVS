execute_process(COMMAND git describe --dirty --always WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE VERSION)
message(STATUS "Using git version: ${VERSION}")
configure_file(${SRC} ${DST} @ONLY)

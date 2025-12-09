file(GLOB_RECURSE SRC_DIR_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../../src/gateway/*.cpp")

add_library(gateway ${SRC_DIR_LIST})
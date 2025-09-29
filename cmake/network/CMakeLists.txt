file(GLOB_RECURSE SRC_DIR_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../../src/network/*.cpp")

add_library(network ${SRC_DIR_LIST})

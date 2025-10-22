file(GLOB_RECURSE SRC_DIR_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../../src/server/*.cpp")

add_library(server ${SRC_DIR_LIST})

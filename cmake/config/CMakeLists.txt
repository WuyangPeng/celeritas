file(GLOB_RECURSE SRC_DIR_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../../src/config/*.cpp")

add_library(config ${SRC_DIR_LIST})

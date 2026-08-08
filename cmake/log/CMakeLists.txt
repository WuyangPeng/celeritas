file(GLOB_RECURSE SRC_DIR_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../../src/log/*.cpp")

add_library(log ${SRC_DIR_LIST})

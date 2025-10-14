file(GLOB_RECURSE SRC_DIR_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../../src/message/*.cpp")

add_library(message ${SRC_DIR_LIST})
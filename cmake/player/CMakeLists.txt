file(GLOB_RECURSE SRC_DIR_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../../src/player/*.cpp")

add_library(player ${SRC_DIR_LIST})
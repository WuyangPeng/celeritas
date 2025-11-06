file(GLOB_RECURSE SRC_DIR_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../../src/path_search/*.cpp")

add_library(path_search ${SRC_DIR_LIST})
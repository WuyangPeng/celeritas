file(GLOB_RECURSE SRC_DIR_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../../src/auth/*.cpp")

add_library(auth ${SRC_DIR_LIST})

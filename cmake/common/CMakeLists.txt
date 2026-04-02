file(GLOB_RECURSE SRC_DIR_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../../src/common/*.cpp")

add_library(common STATIC ${SRC_DIR_LIST})
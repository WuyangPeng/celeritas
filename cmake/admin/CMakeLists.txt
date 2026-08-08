file(GLOB_RECURSE SRC_DIR_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../../src/admin/*.cpp")

add_library(admin ${SRC_DIR_LIST})

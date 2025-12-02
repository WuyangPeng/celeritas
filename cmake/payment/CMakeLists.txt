file(GLOB_RECURSE SRC_DIR_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../../src/payment/*.cpp")

add_library(payment ${SRC_DIR_LIST})
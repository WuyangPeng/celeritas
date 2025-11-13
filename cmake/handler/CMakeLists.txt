file(GLOB_RECURSE SRC_DIR_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../../src/handler/*.cpp")

add_library(handler ${SRC_DIR_LIST})

add_dependencies(handler generate_handler_code)
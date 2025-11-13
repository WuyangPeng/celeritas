file(GLOB_RECURSE SRC_DIR_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../../src/initializer/*.cpp")

add_library(initializer ${SRC_DIR_LIST})

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")

    add_dependencies(initializer generate_handler_header_code)

endif ()
file(GLOB_RECURSE SRC_DIR_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../../src/service_registry/*.cpp")

add_library(service_registry ${SRC_DIR_LIST})

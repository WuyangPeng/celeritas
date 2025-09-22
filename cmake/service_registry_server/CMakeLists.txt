file(GLOB_RECURSE SRC_DIR_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../../src/service_registry_server/*.cpp")

add_executable(service_registry_server ${SRC_DIR_LIST})

target_link_libraries(service_registry_server PRIVATE common)
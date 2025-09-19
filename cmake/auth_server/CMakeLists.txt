file(GLOB_RECURSE SRC_DIR_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../../src/auth_server/*.cpp")

add_executable(auth_server ${SRC_DIR_LIST})
target_link_libraries(auth_server common network protobuf worker_pool)

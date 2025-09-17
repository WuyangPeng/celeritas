file(GLOB_RECURSE SRC_DIR_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../../src/auth_server/*.cpp")

add_executable(AuthServer ${SRC_DIR_LIST})
target_link_libraries(AuthServer common network protobuf)

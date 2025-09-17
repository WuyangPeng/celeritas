file(GLOB_RECURSE SRC_DIR_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../../src/chat_server/*.cpp")

add_executable(ChatServer ${SRC_DIR_LIST})
target_link_libraries(ChatServer common network protobuf)
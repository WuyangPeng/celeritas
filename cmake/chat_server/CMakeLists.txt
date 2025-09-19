file(GLOB_RECURSE SRC_DIR_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../../src/chat_server/*.cpp")

add_executable(chat_server ${SRC_DIR_LIST})
target_link_libraries(chat_server common network protobuf worker_pool)
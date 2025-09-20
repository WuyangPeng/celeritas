file(GLOB_RECURSE SRC_DIR_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../../src/game_server/*.cpp")

add_executable(game_server ${SRC_DIR_LIST})
target_link_libraries(game_server common network protobuf worker_pool database)
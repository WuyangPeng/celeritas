file(GLOB_RECURSE SRC_DIR_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../../src/game_server/*.cpp")

add_executable(GameServer ${SRC_DIR_LIST})
target_link_libraries(GameServer common network protobuf)
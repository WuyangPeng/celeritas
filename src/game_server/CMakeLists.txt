# 使用 GLOB 查找当前目录下所有 .cpp 源文件
file(GLOB SOURCES "*.cpp")

# 将源文件和 Protobuf 生成的源文件一起链接
add_executable(GameServer ${SOURCES} ${PROTO_GAME_SRC})
target_link_libraries(GameServer CommonLib)
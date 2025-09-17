# 使用 GLOB 查找当前目录下所有 .cpp 源文件
file(GLOB SOURCES "*.cpp")

add_executable(ChatServer ${SOURCES})
target_link_libraries(ChatServer CommonLib)
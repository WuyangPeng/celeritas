# 使用 GLOB 查找当前目录下所有 .cpp 源文件
file(GLOB SOURCES "*.cpp")

add_executable(AuthServer ${SOURCES})
target_link_libraries(AuthServer CommonLib)
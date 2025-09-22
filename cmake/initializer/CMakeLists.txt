# 使用 GLOB 查找initializer目录下所有 .cpp 源文件

file(GLOB_RECURSE SRC_DIR_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../../src/initializer/*.cpp")

add_library(initializer ${SRC_DIR_LIST})

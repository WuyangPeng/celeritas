# 使用 GLOB 查找当前目录下所有 .cpp 源文件

file(GLOB_RECURSE SRC_DIR_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../../src/worker_pool/*.cpp")

add_library(worker_pool ${SRC_DIR_LIST})

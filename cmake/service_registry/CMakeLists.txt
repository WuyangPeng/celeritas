# 使用 GLOB 查找service_registry目录下所有 .cpp 源文件

file(GLOB_RECURSE SRC_DIR_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../../src/service_registry/*.cpp")

add_library(service_registry ${SRC_DIR_LIST})

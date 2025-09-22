# 使用 GLOB 查找当前目录下所有 .cpp 源文件

file(GLOB_RECURSE SRC_DIR_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../../src/common/*.cpp")

add_library(common ${SRC_DIR_LIST})

target_link_libraries(common PUBLIC
        libboost_log.so
        libboost_log_setup.so
        libboost_system.so
        libboost_thread.so
        libboost_chrono.so
        libboost_date_time.so
        # ... 其他 common 依赖的 Boost 库
)
# 使用 GLOB 查找当前目录下所有 .cpp 源文件

file(GLOB_RECURSE SRC_DIR_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../../src/common/*.cpp")

add_library(common ${SRC_DIR_LIST})

target_link_libraries(common PUBLIC
        boost_log
        boost_log_setup
        boost_system
        boost_thread
        boost_chrono
        boost_date_time
        # ... 其他 common 依赖的 Boost 库
)
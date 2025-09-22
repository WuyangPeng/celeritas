# 使用 GLOB 查找当前目录下所有 .cpp 源文件

file(GLOB_RECURSE SRC_DIR_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../../src/common/*.cpp")

add_library(common STATIC ${SRC_DIR_LIST})

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")

    target_link_libraries(common PUBLIC
            boost_log-mgw13-mt-d-x64-1_88
            boost_log_setup-mgw13-mt-d-x64-1_88
            boost_system-mgw13-mt-d-x64-1_88
            boost_timer-mgw13-mt-d-x64-1_88
            boost_date_time-mgw13-mt-d-x64-1_88
            boost_thread-mgw13-mt-d-x64-1_88
            boost_charconv-mgw13-mt-d-x64-1_88
            boost_filesystem-mgw13-mt-d-x64-1_88
            boost_regex-mgw13-mt-d-x64-1_88)

else ()

    target_link_libraries(common PUBLIC
            boost_log
            boost_log
            boost_system
            boost_timer
            boost_date_time
            boost_thread
            boost_charconv)

endif ()


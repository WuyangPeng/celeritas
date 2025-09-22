file(GLOB_RECURSE SRC_DIR_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../../src/service_registry_server/*.cpp")

add_executable(service_registry_server ${SRC_DIR_LIST})

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")

    target_link_libraries(service_registry_server PRIVATE
            service_registry
            config initializer
            database
            worker_pool
            network
            common
            boost_system-mgw13-mt-d-x64-1_88
            boost_timer-mgw13-mt-d-x64-1_88
            boost_date_time-mgw13-mt-d-x64-1_88
            boost_thread-mgw13-mt-d-x64-1_88
            boost_charconv-mgw13-mt-d-x64-1_88
            boost_filesystem-mgw13-mt-d-x64-1_88
            boost_regex-mgw13-mt-d-x64-1_88
            boost_log_setup-mgw13-mt-d-x64-1_88
            boost_log-mgw13-mt-d-x64-1_88)

else ()

    target_link_libraries(service_registry_server PRIVATE
            service_registry
            config initializer
            database
            worker_pool
            network
            common
            boost_log
            boost_log_setup
            boost_system
            boost_timer
            boost_date_time
            boost_thread
            boost_charconv
            boost_filesystem
            boost_regex)

endif ()



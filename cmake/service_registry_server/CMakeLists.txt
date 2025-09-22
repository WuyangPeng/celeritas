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
            boost)

else ()

    target_link_libraries(service_registry_server PRIVATE
            service_registry
            config initializer
            database
            worker_pool
            network
            common
            boost)

endif ()



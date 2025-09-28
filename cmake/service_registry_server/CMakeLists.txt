file(GLOB_RECURSE SRC_DIR_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../../src/service_registry_server/*.cpp")

add_executable(service_registry_server ${SRC_DIR_LIST})

target_link_libraries(service_registry_server PRIVATE
        service_registry
        initializer
        config
        database
        worker_pool
        network
        common
        boost
        openssl
        mongo)

file(GLOB CONFIG_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../../config/*")

foreach (config_file IN LISTS CONFIG_FILES)
    add_custom_command(
            TARGET service_registry_server POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:service_registry_server>/config"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${config_file}" "$<TARGET_FILE_DIR:service_registry_server>/config/"
            COMMENT "Copying config files for ${config_file}"
    )
endforeach ()

file(GLOB SERVICE_REGISTRY_CONFIG_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../../config/service_registry/*")

foreach (service_registry_config_file IN LISTS SERVICE_REGISTRY_CONFIG_FILES)
    add_custom_command(
            TARGET service_registry_server POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:service_registry_server>/config/service_registry/"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${service_registry_config_file}" "$<TARGET_FILE_DIR:service_registry_server>/config/service_registry/"
            COMMENT "Copying config files for ${service_registry_config_file}"
    )
endforeach ()

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")

    file(GLOB OPENSSL_DLL_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../../deps/openssl/bin64/*.dll")

    foreach (openssl_dll_file IN LISTS OPENSSL_DLL_FILES)
        add_custom_command(
                TARGET service_registry_server POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "${openssl_dll_file}" "$<TARGET_FILE_DIR:service_registry_server>/"
                COMMENT "Copying dll files for ${openssl_dll_file}"
        )
    endforeach ()

endif ()

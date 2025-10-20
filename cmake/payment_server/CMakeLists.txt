if (CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))

    set(BUILD_TYPE_DIRECTORY debug)

else ()

    set(BUILD_TYPE_DIRECTORY release)

endif ()

file(GLOB_RECURSE SRC_DIR_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../../src/payment_server/*.cpp")

add_executable(payment_server ${SRC_DIR_LIST})

target_link_libraries(payment_server PRIVATE celeritas_lib)

file(GLOB CONFIG_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../../config/*")

foreach (config_file IN LISTS CONFIG_FILES)
    add_custom_command(
            TARGET payment_server POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:payment_server>/config"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${config_file}" "$<TARGET_FILE_DIR:payment_server>/config/"
            COMMENT "Copying config files for ${config_file}"
    )
endforeach ()

file(GLOB SERVICE_REGISTRY_CONFIG_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../../config/payment/*")

foreach (service_registry_config_file IN LISTS SERVICE_REGISTRY_CONFIG_FILES)
    add_custom_command(
            TARGET payment_server POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${service_registry_config_file}" "$<TARGET_FILE_DIR:payment_server>/config/payment/"
            COMMENT "Copying config files for ${service_registry_config_file}"
    )
endforeach ()

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")

    file(GLOB PROTOBUF_DLL_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../../deps/protobuf/bin64/*.dll")

    foreach (protobuf_dll_file IN LISTS PROTOBUF_DLL_FILES)
        add_custom_command(
                TARGET payment_server POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "${protobuf_dll_file}" "$<TARGET_FILE_DIR:payment_server>/"
                COMMENT "Copying dll files for ${protobuf_dll_file}"
        )
    endforeach ()

    file(GLOB OPENSSL_DLL_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../../deps/openssl/bin64/*.dll")

    foreach (openssl_dll_file IN LISTS OPENSSL_DLL_FILES)
        add_custom_command(
                TARGET payment_server POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "${openssl_dll_file}" "$<TARGET_FILE_DIR:payment_server>/"
                COMMENT "Copying dll files for ${openssl_dll_file}"
        )
    endforeach ()

    file(GLOB MONGO_DLL_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../../deps/mongo/bin/*.dll")

    foreach (mongo_dll_file IN LISTS MONGO_DLL_FILES)
        add_custom_command(
                TARGET payment_server POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "${mongo_dll_file}" "$<TARGET_FILE_DIR:payment_server>/"
                COMMENT "Copying dll files for ${mongo_dll_file}"
        )
    endforeach ()

    file(GLOB HIREDIS_DLL_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../../deps/hiredis/build/${BUILD_TYPE_DIRECTORY}/*.dll")

    foreach (hiredis_dll_file IN LISTS HIREDIS_DLL_FILES)
        add_custom_command(
                TARGET payment_server POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "${hiredis_dll_file}" "$<TARGET_FILE_DIR:payment_server>/"
                COMMENT "Copying dll files for ${hiredis_dll_file}"
        )
    endforeach ()

endif ()


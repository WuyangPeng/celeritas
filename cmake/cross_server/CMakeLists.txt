if (CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))

    set(BUILD_TYPE_DIRECTORY debug)

else ()

    set(BUILD_TYPE_DIRECTORY release)

endif ()

file(GLOB_RECURSE SRC_DIR_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../../src/cross_server/*.cpp")

add_executable(cross_server ${SRC_DIR_LIST})

target_link_libraries(cross_server PRIVATE celeritas_lib)

file(GLOB CONFIG_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../../config/*")

foreach (config_file IN LISTS CONFIG_FILES)
    add_custom_command(
            TARGET cross_server POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:cross_server>/config"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${config_file}" "$<TARGET_FILE_DIR:cross_server>/config/"
            COMMENT "Copying config files for ${config_file}"
    )
endforeach ()

file(GLOB SERVICE_REGISTRY_CONFIG_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../../config/cross/*")

foreach (service_registry_config_file IN LISTS SERVICE_REGISTRY_CONFIG_FILES)
    add_custom_command(
            TARGET cross_server POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${service_registry_config_file}" "$<TARGET_FILE_DIR:cross_server>/config/cross/"
            COMMENT "Copying config files for ${service_registry_config_file}"
    )
endforeach ()

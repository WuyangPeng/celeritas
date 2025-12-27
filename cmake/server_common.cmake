if (NOT SERVER_NAME)
    message(FATAL_ERROR "SERVER_NAME must be set before including server_common.cmake")
endif ()

file(GLOB_RECURSE SRC_DIR_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../../src/${SERVER_NAME}_server/*.cpp")

add_executable(${SERVER_NAME}_server ${SRC_DIR_LIST})

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")

    target_link_libraries(${SERVER_NAME}_server PRIVATE celeritas_lib)

else ()

    target_link_libraries(${SERVER_NAME}_server PRIVATE celeritas_lib dl)

    set_target_properties(${SERVER_NAME}_server PROPERTIES LINK_FLAGS "-rdynamic")

endif ()

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")

    file(GLOB BIN_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../../config/bin/*.bytes")

    foreach (bin_file IN LISTS BIN_FILES)
        add_custom_command(TARGET ${SERVER_NAME}_server POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E echo "--- Copying config file: ${bin_file} to $<TARGET_FILE_DIR:${SERVER_NAME}_server>/config/bin"
                COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${SERVER_NAME}_server>/config/bin"
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "${bin_file}" "$<TARGET_FILE_DIR:${SERVER_NAME}_server>/config/bin"
                COMMENT "Copying config files for ${bin_file}")
    endforeach ()

    file(GLOB CONFIG_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../../config/*.xml")

    foreach (config_file IN LISTS CONFIG_FILES)
        add_custom_command(TARGET ${SERVER_NAME}_server POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E echo "--- Copying config file: ${config_file} to $<TARGET_FILE_DIR:${SERVER_NAME}_server>/config/"
                COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${SERVER_NAME}_server>/config"
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "${config_file}" "$<TARGET_FILE_DIR:${SERVER_NAME}_server>/config/"
                COMMENT "Copying config files for ${config_file}")
    endforeach ()

    file(GLOB SERVER_CONFIG_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../../config/${SERVER_NAME}/*.xml")

    foreach (service_config_file IN LISTS SERVER_CONFIG_FILES)
        add_custom_command(TARGET ${SERVER_NAME}_server POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E echo "--- Copying config file: ${service_config_file} to $<TARGET_FILE_DIR:${SERVER_NAME}_server>/"
                COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${SERVER_NAME}_server>/config/${SERVER_NAME}/"
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "${service_config_file}" "$<TARGET_FILE_DIR:${SERVER_NAME}_server>/config/${SERVER_NAME}/"
                COMMENT "Copying config files for ${service_config_file}")
    endforeach ()

    include(${CMAKE_CURRENT_LIST_DIR}/copy_dlls.cmake)
    copy_third_party_dlls(${SERVER_NAME}_server)

endif ()

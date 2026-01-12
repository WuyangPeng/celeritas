include(${CMAKE_CURRENT_SOURCE_DIR}/../copy_dlls.cmake)

file(GLOB_RECURSE SRC_DIR_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../../tests/*.cpp")

add_executable(run_tests ${SRC_DIR_LIST})

target_link_libraries(run_tests celeritas_lib)

add_test(NAME run_tests
        COMMAND run_tests)

copy_third_party_dlls(run_tests)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")

    file(GLOB SERVER_CONFIG_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../../config/tests/*.xml")

    foreach (service_config_file IN LISTS SERVER_CONFIG_FILES)
        add_custom_command(TARGET run_tests POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E echo "--- Copying config file: ${service_config_file} to $<TARGET_FILE_DIR:run_tests>/"
                COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:run_tests>/config/tests/"
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "${service_config_file}" "$<TARGET_FILE_DIR:run_tests>/config/tests/"
                COMMENT "Copying config files for ${service_config_file}")
    endforeach ()

endif ()
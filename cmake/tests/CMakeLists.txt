file(GLOB_RECURSE SRC_DIR_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../../tests/*.cpp")

add_executable(run_tests ${SRC_DIR_LIST})

target_link_libraries(run_tests celeritas_lib)

add_test(NAME run_tests
        COMMAND run_tests)
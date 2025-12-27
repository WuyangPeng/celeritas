macro(copy_third_party_dlls TARGET_NAME)

    if (CMAKE_SYSTEM_NAME STREQUAL "Windows")

        file(GLOB PROTOBUF_DLL_FILES "${PROTOBUF_DIR}/${BIN_DIRECTORY}/*.dll")

        foreach (protobuf_dll_file IN LISTS PROTOBUF_DLL_FILES)

            add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E echo "--- Copying config file: ${protobuf_dll_file} to $<TARGET_FILE_DIR:${TARGET_NAME}>/"
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${protobuf_dll_file}" "$<TARGET_FILE_DIR:${TARGET_NAME}>/"
                    COMMENT "Copying dll files for ${protobuf_dll_file}")

        endforeach ()

        file(GLOB OPENSSL_DLL_FILES "${OPENSSL_DIR}/${BIN_DIRECTORY}/*.dll")

        foreach (openssl_dll_file IN LISTS OPENSSL_DLL_FILES)

            add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E echo "--- Copying config file: ${openssl_dll_file} to $<TARGET_FILE_DIR:${TARGET_NAME}>/"
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${openssl_dll_file}" "$<TARGET_FILE_DIR:${TARGET_NAME}>/"
                    COMMENT "Copying dll files for ${openssl_dll_file}")

        endforeach ()

        file(GLOB MONGO_DLL_FILES "${MONGO_DIR}/${BIN_DIRECTORY}/*.dll")

        foreach (mongo_dll_file IN LISTS MONGO_DLL_FILES)

            add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E echo "--- Copying config file: ${mongo_dll_file} to $<TARGET_FILE_DIR:${TARGET_NAME}>/"
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${mongo_dll_file}" "$<TARGET_FILE_DIR:${TARGET_NAME}>/"
                    COMMENT "Copying dll files for ${mongo_dll_file}")

        endforeach ()

        file(GLOB HIREDIS_DLL_FILES "${HIREDIS_DIR}/build/${BUILD_TYPE_DIRECTORY}/*.dll")

        foreach (hiredis_dll_file IN LISTS HIREDIS_DLL_FILES)

            add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E echo "--- Copying config file: ${hiredis_dll_file} to $<TARGET_FILE_DIR:${TARGET_NAME}>/"
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${hiredis_dll_file}" "$<TARGET_FILE_DIR:${TARGET_NAME}>/"
                    COMMENT "Copying dll files for ${hiredis_dll_file}")
            
        endforeach ()

    endif ()

endmacro()

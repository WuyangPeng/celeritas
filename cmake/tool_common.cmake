if (NOT TOOL_NAME)
    message(FATAL_ERROR "TOOL_NAME must be set before including tool_common.cmake")
endif ()

file(GLOB_RECURSE SRC_DIR_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../../src/${TOOL_NAME}/*.cpp")

add_executable(${TOOL_NAME} ${SRC_DIR_LIST})

target_link_libraries(${TOOL_NAME} PRIVATE common boost proto)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")

    include(${CMAKE_CURRENT_LIST_DIR}/copy_dlls.cmake)
    copy_third_party_dlls(${TOOL_NAME})

endif ()

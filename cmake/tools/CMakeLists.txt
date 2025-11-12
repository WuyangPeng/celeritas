file(GLOB_RECURSE SRC_DIR_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../../src/tools/*.cpp")

add_executable(tools ${SRC_DIR_LIST})

target_link_libraries(tools PRIVATE common boost proto)
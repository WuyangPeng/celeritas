file(GLOB_RECURSE SRC_DIR_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../../src/common/*.cpp")

add_library(common STATIC ${SRC_DIR_LIST})

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_libraries(common PRIVATE stdc++exp)
endif ()
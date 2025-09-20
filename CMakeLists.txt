cmake_minimum_required(VERSION 3.14)
project(celeritas)

# C++标准为C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 包含路径
include_directories(
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
        "${CMAKE_CURRENT_SOURCE_DIR}/proto"
        "${CMAKE_CURRENT_SOURCE_DIR}/deps/boost"
        "${CMAKE_CURRENT_SOURCE_DIR}/deps/protobuf/src"
        "${CMAKE_CURRENT_SOURCE_DIR}/deps/openssl/include"
        # 如果依赖项在本地，添加它们的include路径
)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/boost/stage/lib/)

if (CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))

    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/protobuf/build/bin/debug)

else ()

    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/protobuf/build/bin/release)

endif ()

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")

    set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/deps/protobuf/src")
    set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/deps/protobuf/build/bin/release")
    set(Protobuf_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/deps/protobuf/src")

    # 查找和添加依赖项
    find_package(Protobuf REQUIRED)

    # Protobuf 编译
    # 使用 GLOB_RECURSE 递归查找 proto 目录下所有的 .proto 文件
    file(GLOB_RECURSE PROTO_FILES "${CMAKE_CURRENT_SOURCE_DIR}/proto/*.proto")

    # 自定义输出目录
    set(PROTO_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

    # 遍历你的 .proto 文件，为每个文件生成 C++ 代码
    foreach (proto_file ${PROTO_FILES})

        file(RELATIVE_PATH relative_proto_path "${CMAKE_CURRENT_SOURCE_DIR}" "${proto_file}")

        # 替换文件扩展名为 .pb.cc 和 .pb.h
        string(REPLACE ".proto" ".pb.cc" output_cc_path ${relative_proto_path})
        string(REPLACE ".proto" ".pb.h" output_h_path ${relative_proto_path})

        # 构建完整的输出文件路径
        set(output_cc "${PROTO_OUTPUT_DIR}/${output_cc_path}")
        set(output_h "${PROTO_OUTPUT_DIR}/${output_h_path}")

        # 将生成的文件添加到列表中
        list(APPEND PROTO_SRCS_GENERATED ${output_cc})
        list(APPEND PROTO_HDRS_GENERATED ${output_h})

        # 调用 protoc 编译器
        add_custom_command(
                OUTPUT ${output_cc} ${output_h}
                COMMAND ${Protobuf_PROTOC_EXECUTABLE}
                --proto_path=${CMAKE_CURRENT_SOURCE_DIR} # .proto 文件所在的搜索路径
                --cpp_out=${PROTO_OUTPUT_DIR} # C++ 代码输出目录
                ${proto_file}
                DEPENDS ${proto_file} # 依赖 .proto 文件
                COMMENT "Generating C++ code from ${proto_file}"
                VERBATIM # 确保命令按原样执行
        )

    endforeach ()

else ()

    # 其他平台下的操作
    file(GLOB_RECURSE PROTO_SRCS_GENERATED "src/proto/*.cc")

endif ()

add_library(protobuf
        ${PROTO_SRCS_GENERATED}
)

target_link_libraries(protobuf
        ${PROTOBUF_LIBRARIES}
)

add_subdirectory(cmake/common)
add_subdirectory(cmake/network)
add_subdirectory(cmake/worker_pool)
add_subdirectory(cmake/database)

add_subdirectory(cmake/auth_server)
add_subdirectory(cmake/game_server)
add_subdirectory(cmake/chat_server)

# 安装规则
if (CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))

    install(TARGETS auth_server game_server chat_server DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/bin/debug")

else ()

    install(TARGETS auth_server game_server chat_server DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/bin/release")

endif ()
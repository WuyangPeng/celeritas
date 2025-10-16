cmake_minimum_required(VERSION 3.14)

project(celeritas)

# C++标准为C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (!MSVC)

    # 编译选项和符号可见性
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -fvisibility=hidden")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -fvisibility=hidden")

    # 共享库不允许未定义符号
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-z,defs")

endif ()

if (CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))

    set(BUILD_TYPE_DIRECTORY debug)
    set(BUILD_LIB_SUFFIX d)

else ()

    set(BUILD_TYPE_DIRECTORY release)
    set(BUILD_LIB_SUFFIX)

endif ()

# 包含路径
include_directories(
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
        "${CMAKE_CURRENT_SOURCE_DIR}/proto"
        "${CMAKE_CURRENT_SOURCE_DIR}/deps/boost"
        "${CMAKE_CURRENT_SOURCE_DIR}/deps/protobuf/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/deps/mongo/include/"
        "${CMAKE_CURRENT_SOURCE_DIR}/deps/mongo/include/mongocxx/v_noabi"
        "${CMAKE_CURRENT_SOURCE_DIR}/deps/mongo/include/bsoncxx/v_noabi"
        "${CMAKE_CURRENT_SOURCE_DIR}/deps/hiredis"
        "${CMAKE_CURRENT_SOURCE_DIR}/deps/openssl/include"
        # 如果依赖项在本地，添加它们的include路径
)

# 包含网络库
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")

    link_libraries(ws2_32)

endif ()

# 包含boost库
add_library(boost INTERFACE)

if (MSVC)

    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/boost/stage/lib/)

else ()

    if (CMAKE_SYSTEM_NAME STREQUAL "Linux")

        add_compile_definitions(BOOST_ALL_DYN_LINK)

    endif ()

    set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/deps/boost/stage/lib/cmake")

    find_package(Boost 1.88 REQUIRED CONFIG COMPONENTS filesystem timer date_time thread charconv regex log_setup log program_options)

    target_link_libraries(boost INTERFACE
            Boost::filesystem
            Boost::timer
            Boost::date_time
            Boost::thread
            Boost::charconv
            Boost::regex
            Boost::log_setup
            Boost::log
            Boost::program_options)

    set(CMAKE_BUILD_RPATH "${CMAKE_CURRENT_SOURCE_DIR}/deps/boost/stage/lib")
    set(CMAKE_INSTALL_RPATH "${CMAKE_CURRENT_SOURCE_DIR}/deps/boost/stage/lib")
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

endif ()

# 包含protobuf库
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/protobuf/bin/)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")

    set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/deps/protobuf/bin")
    set(Protobuf_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/deps/protobuf/include")
    set(Protobuf_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/deps/protobuf/lib")

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

    list(APPEND CMAKE_BUILD_RPATH "${CMAKE_CURRENT_SOURCE_DIR}/deps/protobuf/lib64")
    list(APPEND CMAKE_INSTALL_RPATH "${CMAKE_CURRENT_SOURCE_DIR}/deps/protobuf/lib64")

    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/protobuf/lib64)

endif ()

add_library(proto_generated ${PROTO_SRCS_GENERATED})

add_library(proto INTERFACE)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")

    target_link_libraries(proto INTERFACE
            protobuf
            absl_log_internal_message
            absl_log_internal_check_op
            absl_log_internal_nullguard)

else ()

    target_link_libraries(proto INTERFACE
            protobuf
            absl_log_internal_message.dll
            absl_log_internal_check_op.dll
            absl_log_internal_nullguard.dll)

endif ()

# 包含mongo库
add_library(mongo INTERFACE)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")

    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/mongo/lib)

    if (MSVC)

        target_link_libraries(mongo INTERFACE bsoncxx mongocxx)

    else ()

        target_link_libraries(mongo INTERFACE bsoncxx.dll mongocxx.dll)

    endif ()

else ()

    list(APPEND CMAKE_BUILD_RPATH "${CMAKE_CURRENT_SOURCE_DIR}/deps/mongo/lib64")
    list(APPEND CMAKE_INSTALL_RPATH "${CMAKE_CURRENT_SOURCE_DIR}/deps/mongo/lib64")

    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/mongo/lib64)
    target_link_libraries(mongo INTERFACE bsoncxx mongocxx)

endif ()

# 包含hiredis库
add_library(redis INTERFACE)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/hiredis/build/${BUILD_TYPE_DIRECTORY})

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")

    list(APPEND CMAKE_BUILD_RPATH "${CMAKE_CURRENT_SOURCE_DIR}/deps/hiredis/build/${BUILD_TYPE_DIRECTORY}")
    list(APPEND CMAKE_INSTALL_RPATH "${CMAKE_CURRENT_SOURCE_DIR}/deps/hiredis/build/${BUILD_TYPE_DIRECTORY}")

endif ()

target_link_libraries(redis INTERFACE hiredis${BUILD_LIB_SUFFIX})

# 包含openssl库
add_library(openssl INTERFACE)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")

    if (MSVC)

        link_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/openssl/lib)

    else ()

        link_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/openssl/lib64)

    endif ()

endif ()

target_link_libraries(openssl INTERFACE ssl crypto)

# 静态库
add_subdirectory(cmake/common)
add_subdirectory(cmake/network)
add_subdirectory(cmake/worker_pool)
add_subdirectory(cmake/database)
add_subdirectory(cmake/service_registry)
add_subdirectory(cmake/initializer)
add_subdirectory(cmake/config)
add_subdirectory(cmake/message)

# 服务器
add_subdirectory(cmake/service_registry_server)
add_subdirectory(cmake/auth_server)
add_subdirectory(cmake/game_server)
add_subdirectory(cmake/chat_server)

# 安装规则
install(TARGETS service_registry_server auth_server game_server chat_server DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/bin/${BUILD_TYPE_DIRECTORY}")

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/config/"
        DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/bin/${BUILD_TYPE_DIRECTORY}/config/"
        PATTERN "*.xml")

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")

    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/deps/protobuf/bin/"
            DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/bin/${BUILD_TYPE_DIRECTORY}")

    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/deps/openssl/bin64/"
            DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/bin/${BUILD_TYPE_DIRECTORY}")

    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/deps/mongo/bin/"
            DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/bin/${BUILD_TYPE_DIRECTORY}")

    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/deps/hiredis/build/${BUILD_TYPE_DIRECTORY}/"
            DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/bin/${BUILD_TYPE_DIRECTORY}")


endif ()
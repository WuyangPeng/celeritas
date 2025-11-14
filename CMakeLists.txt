cmake_minimum_required(VERSION 3.14)

project(celeritas)

# C++标准为C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))

    set(BUILD_TYPE_DIRECTORY debug)
    set(BUILD_LIB_SUFFIX d)
    set(BUILD_LIB_TYPE d)

else ()

    set(BUILD_TYPE_DIRECTORY release)
    set(BUILD_LIB_SUFFIX)
    set(BUILD_LIB_TYPE r)

endif ()

if (MSVC)

    add_compile_definitions(_WIN32_WINNT=0x0A00)

    set(LIB_DIRECTORY lib)
    set(BIN_DIRECTORY bin)
    set(LIB_PREFIX lib)
    set(MSVC_PREFIX msvc_)
    set(MSVC_SUFFIX _${BUILD_TYPE_DIRECTORY})

else ()

    # 编译选项和符号可见性
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -fvisibility=hidden")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -fvisibility=hidden")

    if (CMAKE_SYSTEM_NAME STREQUAL "Linux")

        # 共享库不允许未定义符号
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-z,defs")

    else ()

        # 共享库不允许未定义符号
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined")

    endif ()


    set(LIB_DIRECTORY lib64)
    set(BIN_DIRECTORY bin64)
    set(LIB_PREFIX)
    set(MSVC_PREFIX)
    set(MSVC_SUFFIX)

endif ()

set(BOOST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/deps/boost")
set(PROTOBUF_DIR "${CMAKE_CURRENT_SOURCE_DIR}/deps/${MSVC_PREFIX}protobuf${MSVC_SUFFIX}")
set(MONGO_DIR "${CMAKE_CURRENT_SOURCE_DIR}/deps/${MSVC_PREFIX}mongo${MSVC_SUFFIX}")
set(HIREDIS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/deps/hiredis")
set(OPENSSL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/deps/openssl")

# 包含路径
include_directories(
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
        "${CMAKE_CURRENT_SOURCE_DIR}/proto"
        "${BOOST_DIR}"
        "${PROTOBUF_DIR}/include"
        "${MONGO_DIR}/include/"
        "${MONGO_DIR}/include/mongocxx/v_noabi"
        "${MONGO_DIR}/include/bsoncxx/v_noabi"
        "${HIREDIS_DIR}"
        "${OPENSSL_DIR}/include"
        # 如果依赖项在本地，添加它们的include路径
)

# 包含网络库
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")

    link_libraries(ws2_32)

endif ()

# 包含boost库
add_library(boost INTERFACE)

set(BOOST_LIB_DIR "${BOOST_DIR}/stage/lib/")

if (MSVC)

    link_directories(${BOOST_LIB_DIR}/)

else ()

    if (CMAKE_SYSTEM_NAME STREQUAL "Linux")

        add_compile_definitions(BOOST_ALL_DYN_LINK)

    endif ()

    set(CMAKE_PREFIX_PATH "${BOOST_LIB_DIR}/cmake")

    find_package(Boost 1.88 REQUIRED CONFIG COMPONENTS
            filesystem
            timer
            date_time
            thread
            charconv
            regex
            log_setup
            log
            program_options
            url)

    target_link_libraries(boost INTERFACE
            Boost::filesystem
            Boost::timer
            Boost::date_time
            Boost::thread
            Boost::charconv
            Boost::regex
            Boost::log_setup
            Boost::log
            Boost::program_options
            Boost::url)

endif ()

# 包含protobuf库
set(PROTOBUF_BIN_DIR "${PROTOBUF_DIR}/${BIN_DIRECTORY}/")

link_directories(${PROTOBUF_BIN_DIR})

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")

    set(CMAKE_PREFIX_PATH "${PROTOBUF_BIN_DIR}")
    set(Protobuf_INCLUDE_DIR "${PROTOBUF_DIR}/include")
    set(Protobuf_LIBRARIES "${PROTOBUF_DIR}/${LIB_DIRECTORY}")

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

    file(GLOB_RECURSE PROTO_SRCS_GENERATED "src/proto/*.cc")

    link_directories(${PROTOBUF_DIR}/lib64)

endif ()

add_library(proto_generated ${PROTO_SRCS_GENERATED})

add_library(proto INTERFACE)

if (MSVC)

    add_compile_definitions(PROTOBUF_USE_DLLS)
    add_compile_definitions(ABSL_CONSUME_DLL)

    link_directories(${PROTOBUF_DIR}/${LIB_DIRECTORY})

    target_link_libraries(proto INTERFACE
            libprotobuf${BUILD_LIB_SUFFIX}
            abseil_dll)

else ()

    target_link_libraries(proto INTERFACE
            protobuf
            absl_log_internal_message
            absl_log_internal_check_op
            absl_log_internal_nullguard)

endif ()

# 包含mongo库
add_library(mongo INTERFACE)

link_directories(${MONGO_DIR}/${LIB_DIRECTORY})

if (MSVC)

    target_link_libraries(mongo INTERFACE
            bsoncxx-v_noabi-${BUILD_LIB_TYPE}hi-md${BUILD_LIB_SUFFIX}
            mongocxx-v_noabi-${BUILD_LIB_TYPE}hi-md${BUILD_LIB_SUFFIX})

else ()

    target_link_libraries(mongo INTERFACE
            bsoncxx
            mongocxx)

endif ()

# 包含hiredis库
add_library(redis INTERFACE)

link_directories(${HIREDIS_DIR}/build/${BUILD_TYPE_DIRECTORY})

target_link_libraries(redis INTERFACE hiredis${BUILD_LIB_SUFFIX})

# 包含openssl库
add_library(openssl INTERFACE)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")

    link_directories(${OPENSSL_DIR}/${LIB_DIRECTORY})

endif ()

target_link_libraries(openssl INTERFACE ${LIB_PREFIX}ssl ${LIB_PREFIX}crypto)

# 静态库
add_subdirectory(cmake/common)

# 辅助
add_subdirectory(cmake/tools)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")

    # 定义代码生成的目录
    set(HANDLER_GENERATED_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/handler/generated")
    set(INITIALIZER_GENERATED_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/initializer/generated")

    add_custom_target(generate_handler_code

            # 添加创建目录的命令
            COMMAND ${CMAKE_COMMAND} -E make_directory ${HANDLER_GENERATED_DIR}

            # COMMAND $<TARGET_FILE:tools> 会获取编译好的 tools 可执行文件的路径
            COMMAND $<TARGET_FILE:tools>

            # 将生成目录作为参数传递给 tools
            --process=generate_handler
            --proto_directory=${CMAKE_CURRENT_SOURCE_DIR}/proto
            --output_directory=${HANDLER_GENERATED_DIR}
            --template_directory=${CMAKE_CURRENT_SOURCE_DIR}/data/template

            # 依赖于 tools 目标本身，确保 tools 在执行前已编译完成
            DEPENDS tools

            COMMENT "Running tools to generate handler source code into: ${HANDLER_GENERATED_DIR}"
    )

    add_custom_target(generate_handler_header_code

            # 添加创建目录的命令
            COMMAND ${CMAKE_COMMAND} -E make_directory ${HANDLER_GENERATED_DIR}

            # COMMAND $<TARGET_FILE:tools> 会获取编译好的 tools 可执行文件的路径
            COMMAND $<TARGET_FILE:tools>

            # 将生成目录作为参数传递给 tools
            --process=generate_handler_header
            --handler_directory=${CMAKE_CURRENT_SOURCE_DIR}/src/handler
            --output_directory=${HANDLER_GENERATED_DIR}
            --template_directory=${CMAKE_CURRENT_SOURCE_DIR}/data/template

            # 依赖于 tools 目标本身，确保 tools 在执行前已编译完成
            DEPENDS tools generate_handler_code

            COMMENT "Running tools to generate handler header code into: ${HANDLER_GENERATED_DIR}"
    )

    add_custom_target(generate_register_handler_helper_code

            # 添加创建目录的命令
            COMMAND ${CMAKE_COMMAND} -E make_directory ${HANDLER_GENERATED_DIR}

            # COMMAND $<TARGET_FILE:tools> 会获取编译好的 tools 可执行文件的路径
            COMMAND $<TARGET_FILE:tools>

            # 将生成目录作为参数传递给 tools
            --process=generate_register_handler_helper
            --handler_directory=${CMAKE_CURRENT_SOURCE_DIR}/src/handler
            --output_directory=${INITIALIZER_GENERATED_DIR}
            --template_directory=${CMAKE_CURRENT_SOURCE_DIR}/data/template

            # 依赖于 tools 目标本身，确保 tools 在执行前已编译完成
            DEPENDS tools generate_handler_code generate_handler_header_code

            COMMENT "Running tools to generate register handler helper code into: ${INITIALIZER_GENERATED_DIR}"
    )

endif ()

# 静态库
add_subdirectory(cmake/config)
add_subdirectory(cmake/worker_pool)
add_subdirectory(cmake/message)
add_subdirectory(cmake/database)
add_subdirectory(cmake/network)
add_subdirectory(cmake/service_registry)
add_subdirectory(cmake/handler)
add_subdirectory(cmake/initializer)
add_subdirectory(cmake/server)

add_library(celeritas_lib INTERFACE)

target_link_libraries(celeritas_lib INTERFACE
        server
        initializer
        handler
        service_registry
        network
        database
        message
        worker_pool
        config
        common
        boost
        openssl
        mongo
        redis
        proto_generated
        proto)

# 服务器
add_subdirectory(cmake/service_registry_server)
add_subdirectory(cmake/auth_server)
add_subdirectory(cmake/gateway_server)
add_subdirectory(cmake/player_server)
add_subdirectory(cmake/logic_server)
add_subdirectory(cmake/game_server)
add_subdirectory(cmake/payment_server)
add_subdirectory(cmake/admin_server)
add_subdirectory(cmake/cross_server)
add_subdirectory(cmake/chat_server)

# 安装规则
install(TARGETS
        service_registry_server
        auth_server
        gateway_server
        player_server
        logic_server
        game_server
        payment_server
        admin_server
        cross_server
        chat_server
        DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/bin/${BUILD_TYPE_DIRECTORY}")

set(APP_RUNTIME_RPATH "$ORIGIN/lib")

set_target_properties(
        service_registry_server
        auth_server
        gateway_server
        player_server
        logic_server
        game_server
        payment_server
        admin_server
        cross_server
        chat_server
        PROPERTIES INSTALL_RPATH "${APP_RUNTIME_RPATH}")

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/config/"
        DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/bin/${BUILD_TYPE_DIRECTORY}/config/"
        PATTERN "*.xml")

set(THIRD_PARTY_DLLS)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")

    # 定义库的最终安装目标目录
    set(APP_LIB_INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin/${BUILD_TYPE_DIRECTORY}")

    file(GLOB _dlls
            "${OPENSSL_DIR}/${BIN_DIRECTORY}/*.dll*"
            "${PROTOBUF_DIR}/${BIN_DIRECTORY}/*.dll*"
            "${MONGO_DIR}/${BIN_DIRECTORY}/*.dll*"
            "${HIREDIS_DIR}/build/${BUILD_TYPE_DIRECTORY}/libhiredis${BUILD_LIB_SUFFIX}.dll*")

else ()

    # 定义库的最终安装目标目录
    set(APP_LIB_INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin/${BUILD_TYPE_DIRECTORY}/lib")

    file(GLOB _dlls
            "${BOOST_DIR}/stage/lib/*.so*"
            "${PROTOBUF_DIR}/${LIB_DIRECTORY}/*.so*"
            "${MONGO_DIR}/${LIB_DIRECTORY}/*.so*"
            "${HIREDIS_DIR}/build/${BUILD_TYPE_DIRECTORY}/libhiredis${BUILD_LIB_SUFFIX}.so*")
endif ()

list(APPEND THIRD_PARTY_DLLS ${_dlls})

install(FILES ${THIRD_PARTY_DLLS}
        DESTINATION ${APP_LIB_INSTALL_DIR}
        PERMISSIONS
        OWNER_READ
        OWNER_WRITE
        OWNER_EXECUTE
        GROUP_READ
        GROUP_EXECUTE
        WORLD_READ
        WORLD_EXECUTE)
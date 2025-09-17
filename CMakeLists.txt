cmake_minimum_required(VERSION 3.31)
project(celeritas)

# C++标准为C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 包含路径
include_directories(
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/proto"
        # 如果依赖项在本地，添加它们的include路径
)

# 查找和添加依赖项
find_package(Boost REQUIRED components system thread)

find_package(Protobuf REQUIRED)

# Protobuf 编译
file(GLOB COMMON_PROTO_FILES "proto/common/*.proto")
file(GLOB GAME_PROTO_FILES "proto/game/*.proto")

protobuf_generate_cpp(PROTO_COMMON_SRC PROTO_COMMON_HDRS ${COMMON_PROTO_FILES})
protobuf_generate_cpp(PROTO_GAME_SRC PROTO_GAME_HDRS ${GAME_PROTO_FILES})

# 定义所有进程共享的库
# 使用 GLOB 查找源文件，以便自动包含新增的文件
file(GLOB COMMON_SRC "src/common/*.cpp")
file(GLOB DB_SRC "src/db/*.cpp")
file(GLOB WORKER_POOL_SRC "src/worker_pool/*.cpp")
file(GLOB NETWORK_SRC "src/network/*.cpp")
file(GLOB IPC_SRC "src/network/ipc/*.cpp")

add_library(CommonLib
        ${COMMON_SRC}
        ${DB_SRC}
        ${WORKER_POOL_SRC}
        ${NETWORK_SRC}
        ${IPC_SRC}
        ${PROTO_COMMON_SRC}
)
target_link_libraries(CommonLib
        Boost::system
        Boost::thread
        ${PROTOBUF_LIBRARIES}
)

# 添加子目录，让它们各自管理自己的编译
add_subdirectory(src/auth_server)
add_subdirectory(src/game_server)
add_subdirectory(src/chat_server)

# 安装规则（可选）
install(TARGETS AuthServer GameServer ChatServer DESTINATION bin)


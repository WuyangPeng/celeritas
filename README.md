[⬅️ 返回主文档](../README.md)

# celeritas

本设计旨在为一款高性能游戏服务器提供一个基础框架。该框架将使用C++23标准和boost库，实现一个异步、多线程、高吞吐量的系统。

## supported Platforms（支持平台）

- linux
- windows

## supported Compiler（支持编译器）

- gcc 14.2 (linux)
- MinGW 15.2 (windows)
- Visual Studio 2022 (windows)

## dependency Library（依赖库）

- boost 1.88.0
- protobuf 32.1
- openssl 3.5.1
- mongo v4.1
- hiredis v1.3.0
- luban v4.5.0

## lib（静态库）

- common 通用
- config 配置
- message 消息
- database 数据库
- network 网络
- service_registry 服务注册
- auth 认证
- payment 支付
- gateway 网关
- player 玩家
- handler 处理器
- initializer 初始化

## assist（辅助）

- generate_handler_tools 生成处理器工具
- generate_database_tools 生成数据库工具

## server（服务器）

- service registry 服务注册中心
- auth 认证
- payment 支付
- admin 后台
- chat 聊天
- gateway 网关
- player 玩家
- logic 逻辑
- game 游戏
- battle 战斗
- cross 跨服
- log 日志

## port allocation（端口分配）

* **百位服务**
    - 服务注册中心 0
    - 认证 1
    - 支付 2
    - 后台 3
    - 聊天 4
    - 网关 5
    - 玩家 6
    - 逻辑 7
    - 游戏 8
    - 战斗 9
    - 跨服 10
    - 日志 11


* **十位实例**
    - 0 - 9


* **个位协议**
    - TCP 0
    - HTTP 1
    - Websocket 2
    - TCP + SSL 5
    - HTTP + SSL 6
    - Websocket + SSL 7

## [feature（实现功能）](docs/feature.md)

## [docker（安装docker）](docs/docker.md) 

[⬅️ 返回主文档](../README.md)
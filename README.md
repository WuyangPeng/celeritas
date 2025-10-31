# celeritas

本设计旨在为一款高性能游戏服务器提供一个基础框架。该框架将使用C++20标准和boost库，实现一个异步、多线程、高吞吐量的系统。

## Supported Platforms（支持平台）:

- windows
- linux

## Supported Compiler（支持编译器）:

- MinGW 15.2 (windows)
- Visual Studio 2022  (windows)
- gcc 14.2  (linux)

## Dependency Library（依赖库）:

- boost 1.88.0
- protobuf 32.1
- openssl 3.5.1
- mongo v4.1
- hiredis v1.3.0

## lib（静态库）:

- server 服务器
- initializer 初始化
- service_registry 服务注册
- network 网络
- database 数据库
- message 消息
- worker_pool 工作池
- config 配置
- common 通用

## Server（服务器）:

- service registry 服务注册中心
- auth 认证
- payment 支付
- admin 后台
- chat 聊天
- gateway 网关
- player 玩家
- logic 逻辑
- game 游戏
- cross 跨服

## Port Allocation（端口分配）

- 百位服务
    - 服务注册中心 0
    - 认证 1
    - 支付 2
    - 后台 3
    - 聊天 4
    - 网关 5
    - 玩家 6
    - 逻辑 7
    - 游戏 8
    - 跨服 9
- 十位实例
    - 0 - 9
- 个位协议
    - TCP 0
    - HTTP 1
    - Websocket 2
    - TCP + SSL 5
    - HTTP + SSL 6
    - Websocket + SSL 7

## Feature（实现功能）

### service registry（服务注册中心）

- 🚀 服务注册（Service Registration）
    - 提供者注册： 允许服务提供者在启动时向注册中心报告自己的网络地址（IP、端口）和元数据（服务名、版本等）。
    - 心跳机制： 服务提供者需要定期向注册中心发送心跳，证明自己还“活着”并且健康可用。这是维持注册信息新鲜度的关键。

- 🔍 服务发现（Service Discovery）
    - 查询接口： 允许服务消费者根据服务名称向注册中心查询该服务所有可用实例的列表和它们的网络地址。这是服务间通信的基础。
    - 列表缓存： 通常会支持服务消费者缓存查询到的实例列表，减少注册中心的压力，提高查询速度。

- 🩺 健康检查与剔除（Health Check & Eviction）
    - 实例健康判断： 注册中心通过心跳机制或主动探查来判断服务实例的健康状况。
    - 不健康实例剔除： 如果一个实例在一定时间内没有发送心跳，或者健康检查失败，注册中心需要将其从可用服务列表中自动剔除，防止请求被路由到故障的实例上。

- 🔔 变更通知（Change Notification）
    - 定时拉取： 服务消费者不会等待注册中心主动通知。它会主动、周期性地向服务注册中心发起查询请求，拉取目标服务最新的可用实例列表。

- 🛡️ 高可用与集群（High Availability & Clustering）
    - 集群部署： 注册中心本身作为关键基础设施，必须支持集群部署，防止单点故障。
    - 数据同步： 集群中的注册中心节点之间需要保持注册信息的同步和一致性。

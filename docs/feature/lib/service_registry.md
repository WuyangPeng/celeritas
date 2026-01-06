# 服务注册（service registry）

服务注册库提供了一套客户端接口和机制，用于服务在分布式环境中进行注册、发现和健康检查，是实现微服务间动态通信的基础。

## 基础定义与数据结构 (Basic Definitions & Data Structures)

* **🩺 健康检查级别 (`health_check_level_type`)**
    - **作用**：定义了服务实例的健康状态级别。
    - **特点**：枚举类型，包含 `health` (健康), `warning` (警告), `error` (错误)。


* **❤️ 健康检查 (`health_check`)**
    - **作用**：封装了服务实例的健康检查信息。
    - **特点**：包含实例ID和健康级别，支持JSON序列化，用于在服务和注册中心之间传递健康状态。


* **🔌 协议端口 (`protocol_port`)**
    - **作用**：封装了服务支持的网络协议类型和对应的端口号。
    - **特点**：包含 `server_network_type` (网络协议类型) 和 `port` (端口号)。


* **📝 服务信息 (`service_info`)**
    - **作用**：封装了单个服务实例的所有必要信息，是服务注册和发现的核心数据结构。
    - **特点**：包含 `instance_id` (实例唯一ID)、`service_name` (服务名称)、`host` (主机地址)、`game_server_id` (游戏服ID)、
      `last_heartbeat` (上次心跳时间)、`protocol_port_container` (协议端口列表)、`health_check_level_type` (健康检查级别)和
      `start_server_time` (启动时间)。

## 核心功能 (Core Functionality)

* **⚙️ 服务注册客户端 (`service_registry`)**
    - **作用**：作为静态类，提供核心的服务注册、发现和管理功能。
    - **功能**:
        - **注册服务**：向服务注册中心注册当前服务实例的详细信息。
        - **发现服务**：根据服务名称查询并获取该服务所有可用实例的列表。
        - **移除服务**：从注册中心移除指定的服务实例。
        - **更新健康状态**：更新指定服务实例的健康检查级别。

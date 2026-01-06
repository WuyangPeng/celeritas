# config（配置）

配置模块定义了服务器启动所需的核心配置数据结构。

## basic config types（基础配置类型）

* **🌐 服务器网络类型（`server_network_type`）**
    - **作用**：一个枚举类型，用于明确表示服务器支持的网络通信协议。它是网络配置的基础。
    - **枚举值**：`tcp`、`http`、`websocket`、`tcp_ssl`、`https`、`websocket_secure`。
    - **功能**：提供了全局函数`get_server_network_type(const std::string&)`，用于将协议名称字符串转换为对应的枚举值。


* **📊 数据库类型（`database_type`）**
    - **作用**：一个枚举类型，用于明确表示框架支持的数据库类型。它是数据库配置的基础。
    - **枚举值**：`mysql`、`mongodb`、`redis`。
    - **功能**：提供了全局函数`get_database_type(const std::string&)`，用于将数据库类型名称字符串转换为对应的枚举值。

## local configuration structures（局部配置结构）

* **🔌 服务器网络配置（`server_network_config`）**
    - **作用**：封装单个服务器实例的一个网络监听配置，即一个协议类型和对应的端口号。
    - **成员**：
        - **server_network_type**：网络协议类型 (`server_network_type`)。
        - **port**：监听端口号。


* **📜 日志级别配置（`logger_level_config`）**
    - **作用**：集中配置日志系统的默认级别和控制台输出级别。
    - **成员**：
        - **default_level**：默认日志级别。
        - **console_level**：控制台日志级别。
    - **功能**：允许设置和获取默认和控制台的日志级别。


* **🔭 服务注册配置（`service_registry_config`）**
    - **作用**： 包含服务在向注册中心注册或服务消费者连接注册中心时所需的配置信息。
    - **成员**：
        - **name**：服务注册中心的名称。
        - **host**：服务注册中心的主机地址。
        - **port**：服务注册中心的端口号。


* **🩺 健康检查 URL 配置（`health_check_url_config`）**
    - **作用**： 封装了服务器健康检查`URL`的配置信息。
    - **成员**：
        - **url**：健康检查请求的`URL`路径。
        - **interval**：检查间隔时间。
        - **timeout**：检查超时时间。


* **💾 数据库连接配置（`database_config`）**
    - **作用**：封装一个数据库连接所需的全部配置信息。
    - **成员**：包含数据库类型(`database_type`)、连接名、主机、端口、用户名、密码、数据库名称、连接池大小（最小/最大连接数）和超时时间。


* **🪵 日志配置（`logger_config`）**
    - **作用**：包含单个日志通道的配置信息，如文件输出、旋转大小和通道级别。
    - **成员**：包含日志通道名称、文件目录、文件名、日志级别和文件旋转大小等。


* **🌍 全局配置 (`global_config`)**
    - **作用**：`global_config` 类用于存储全局配置选项。

## aggregate and top-level configurations（聚合配置结构与顶级配置）

* **⚙️ 服务器配置（`server_config`）**
    - **作用**：包含一个服务器实例的所有全局配置信息。
    - **成员**：
        - **instance_id**：服务器实例的唯一ID。
        - **service_name**：服务名称（例如：`"auth"`, `"game"`）。
        - **server_network_config**：容器，包含一个或多个`server_network_config`对象，定义了该服务器监听的所有网络端口和协议。
        - **game_server_id**：游戏服ID。
        - **host**：服务器的IP地址或主机名。
        - **worker_pool_size**：工作池线程数量。


* **🧩 应用配置（`app_config`）**
    - **作用**：作为整个服务器应用配置的总管理器。它是所有配置类的聚合器，负责统一加载和运行时访问所有子系统配置。
    - **核心职责**：
        - **配置聚合**：内部存储`server_config`、`logger_level_config`，以及多个服务注册、数据库和日志的配置容器。
        - **配置加载**：提供一系列公共`load_xxx_config(const std::string& filename)`方法，负责从配置文件中解析并初始化所有子配置。
        - **配置访问**：提供公共`get_xxx_config()`方法，用于在程序运行时获取已加载的配置实例或配置容器。

## 游戏数据配置 (Game Data Configuration)

* **🎲 游戏配置 (`game_config`)**
    - **作用**：定义和管理游戏世界的核心配置，例如游戏规则、数值和全局参数。


* **📚 游戏数据表 (`game_tables`)**
    - **作用**：作为所有游戏数据表（如道具、角色、技能等）的容器，负责加载和提供对这些数据的访问。


* **📦 容器配置 (`container_config<Element>`)**
    - **作用**：一个模板化的配置类，用于加载和管理一组相同类型的配置元素。


* **⚖️ 权重配置 (`weight`)**
    - **作用**：用于管理和操作一组带权重的元素。
    - **功能**：
        - **`add_element(id, weights)`**：添加一个带权重的元素。
        - **`get_weights()`**：获取所有元素的权重列表。
        - **`get_id(index)`**：根据索引获取元素的ID。
        - **`clear()`**：清空所有元素。

## luban configuration (Luban 配置)

* **📊 Luban 总表 (`tables`)**
    - **作用**：Luban 工具生成的代码入口类，管理所有生成的配置表容器。
    - **功能**：提供 `load` 方法从 `ByteBuf` 加载所有配置数据。


* **📝 配置表结构 (`config_bean`)**
    - **作用**：定义了所有配置表的基础结构。
    - **特点**：所有具体的配置表（如 `item_config`, `develop_config` 等）都由 Luban
      工具自动生成，并继承自基础配置类。这些类包含了具体的业务字段，用于在运行时访问配置数据。

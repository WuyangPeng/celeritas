# celeritas

本设计旨在为一款高性能游戏服务器提供一个基础框架。该框架将使用C++20标准和boost库，实现一个异步、多线程、高吞吐量的系统。

## supported Platforms（支持平台）：

- windows
- linux

## supported Compiler（支持编译器）：

- MinGW 15.2 (windows)
- Visual Studio 2022  (windows)
- gcc 14.2  (linux)

## dependency Library（依赖库）：

- boost 1.88.0
- protobuf 32.1
- openssl 3.5.1
- mongo v4.1
- hiredis v1.3.0

## lib（静态库）：

- common 通用
- config 配置
- worker_pool 工作池
- message 消息
- database 数据库
- network 网络
- service_registry 服务注册
- handler 处理器
- initializer 初始化
- server 服务器

## assist（辅助）：

- generate_handler_tools 生成处理器工具
- generate_database_tools 生成数据库工具

## server（服务器）：

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
    - 跨服 9
* **十位实例**
    - 0 - 9
* **个位协议**
    - TCP 0
    - HTTP 1
    - Websocket 2
    - TCP + SSL 5
    - HTTP + SSL 6
    - Websocket + SSL 7

## Feature（实现功能）

### lib（静态库）

#### common（通用）

通用模块包含了框架中常用的基础工具类和宏，以提供异常处理、日志、缓冲区管理、定时器、随机数生成等核心功能。

* **❌ 框架基础异常（`celeritas_error`）**
    - **作用**：框架自定义的基础异常类。
    - **特点**：继承自`std：：runtime_error`，用于包装和抛出框架运行时的错误信息，是大多数错误处理的基础。


* **📜 日志系统（`logger`）**
    - **作用**：基于`boost：：log`实现的统一日志记录接口。
    - **功能**：
        - 支持全局日志级别初始化的`init_global`。
        - 支持控制台输出初始化的`init_console`。
        - 支持文件输出的`init_file`，可指定不同的日志通道`channel_name`。
        - 通过`get_default(level)` 或`get(channel_name,level)` 获取指定通道的日志实例。
    - **宏定义**：
        - `LOG(level)`：记录到默认日志通道。
        - `LOG_CHANNEL(channel, level)`：：记录到指定通道。
    - **日志信息增强**：
        - 日志宏利用`C++20`的`std：：source_location：：current()`自动添加函数名`function_name()`、
          文件名`file_name()`和行号`line()`到日志记录中，极大地增强了调试能力。


* **🛡️ 无异常安全调用与日志记录（`noexcept_safe_call_and_log`）**
    - **作用**：作为一个通用的函数模板，用于安全地调用可能抛出异常的函数f，并确保在发生异常时能记录日志，同时防止异常逃逸出
      `noexcept`函数（如析构函数）。
    - **异常处理**：
        - 捕获`std：：exception`及其子类，记录日志，并输出`error.what()`。
        - 捕获所有其他未知异常(...)，并记录日志。
        - 内部嵌套的`try-catch`用于忽略日志记录本身可能失败的情况。


* **💾 缓冲区数据结构（`buffer_pool_data`）**
    - **作用**：封装底层的 `std：：vector<char>`，作为缓冲区池管理的基本数据单元。
    - **特点**：包含数据指针 `data()`和缓冲区大小`size()`，并提供有效性检查`is_effective()`。
    - **功能**：支持从`std：：string`或`std：：span<const char>`写入数据。


* **💧 缓冲区池（`buffer_pool`）**
    - **作用**：集中管理和复用固定大小的内存缓冲区，以减少频繁的堆内存分配和释放，提升性能。
    - **核心功能**：**获取** (`acquire`)、**归还** (`release`) 和定期**回收** (`reclaim`) 闲置缓冲区。
    - **模式**：采用静态实现，确保全局唯一和集中管理。


* **🔒 缓冲区 RAII 守卫（`buffer_guard`）**
    - **作用**：遵循 RAII 原则，用于**安全管理**从`buffer_pool`中获取的缓冲区。
    - **安全**：在析构时**自动**将其归还 `release()`到 `buffer_pool`，其析构函数标记为`noexcept`，并依赖
      `noexcept_safe_call_and_log`确保归还操作的安全性。


* **⏰ 定时器基类（`timer_base`）**
    - **作用**：基于`boost：：asio：：steady_timer`实现的**周期性**异步定时器基类。
    - **特点**：继承自`std：：enable_shared_from_this`，要求通过`std：：shared_ptr`进行管理，确保在异步操作进行时对象不会被销毁。
    - **使用**：子类需要实现纯虚函数`execute_timer_task()`来定义定时器到期时执行的业务逻辑。
    - **运行机制**：使用`wait_for_next_tick()`在定时器到期后（在`next_tick`中）重新设置下一次等待，实现周期性执行。
    - **安全**：在析构函数中使用`noexcept_safe_call_and_log`确保安全调用`stop()`并取消定时器，防止异常逃逸。
    - **错误处理**：`on_timer_elapsed()`内部包含`try-catch`块，用于捕获和记录`execute_timer_task()`执行过程中抛出的所有异常。


* **🎲 随机数工具（`random_helper`）**
    - **作用**：提供线程安全的随机数生成工具，封装了C++`<random>`库。
    - **线程安全**：使用`thread_local std：：mt19937 engine`确保每个线程拥有独立的随机数引擎。
    - **功能**：
        - **生成指定范围的整数（int）**：[0, end) 或 [begin, end)。
        - **生成指定范围的浮点数（float, double）**：[0.0, 1.0) 或 [begin, end)。
        - **生成布尔值（bool）**：可指定返回 true 的概率p。
        - **生成服从正态分布的 double 值**：（可指定均值和标准差）。


* **⚙️ 命令行配置解析（`command_line_config`）**
    - **作用**：用于解析服务器启动时的命令行参数。
    - **依赖**：基于`boost：：program_options`库实现。
    - **功能**：
        - 自动添加 `--help, -h` 选项，并处理退出请求。
        - 提供 `get<T>(key)` 模板方法获取配置，键不存在时抛出 `celeritas_error` 异常。


* **🌐 会话基类（`session`）**
    - **作用**：定义了网络通信会话的抽象接口。它是所有具体网络连接（如`TCP/HTTP/WebSocket`会话）的基类。
    - **特点**：
      继承自`std::enable_shared_from_this`，要求通过`std::shared_ptr`进行管理，以确保在异步读写操作进行时对象不会被意外销毁。


* **📦 资源加载器基类（`resource_loader_base`）**
    - **作用**：定义了所有资源加载器的抽象基类。
    - **特点**：继承自`std::enable_shared_from_this`，要求通过`std::shared_ptr`进行管理，以支持在异步加载任务中的安全生命周期管理。


* **⚙️ 框架主入口基类（`celeritas_main`）**
    - **作用**：作为所有服务器类型（如`auth`,`game`, `gateway`等）的主入口点的抽象基类。它封装了服务器启动的通用流程。
    - **构造**：通过构造函数接收一个`server_type`，用于标识具体的服务器类型。

#### config（配置）

配置模块定义了服务器启动所需的核心配置数据结构。

##### **基础配置类型（`Basic Config Types`）**

* **🌐 服务器网络类型（`server_network_type`）**
    - **作用**：一个枚举类型，用于明确表示服务器支持的网络通信协议。它是网络配置的基础。
    - **枚举值**：`unknown`、`tcp`、`http`、`websocket`、`tcp_ssl`、`https`、`websocket_secure`。
    - **功能**：提供了全局函数`get_server_network_type(const std::string&)`，用于将协议名称字符串转换为对应的枚举值。


* **📊 数据库类型（`database_type`）**
    - **作用**：一个枚举类型，用于明确表示框架支持的数据库类型。它是数据库配置的基础。
    - **枚举值**：`unknown`、`mysql`、`mongodb`、`redis`。
    - **功能**：提供了全局函数`get_database_type(const std::string&)`，用于将数据库类型名称字符串转换为对应的枚举值。

##### 局部配置结构（`Local Configuration Structures`）

* **🔌 服务器网络配置（`server_network_config`）**
    - **作用**：封装单个服务器实例的一个网络监听配置，即一个协议类型和对应的端口号。
    - **成员**：
        - **server_network_type_**：网络协议类型 (`server_network_type`)。
        - **port_**：监听端口号。


* **📜 日志级别配置（`logger_level_config`）**
    - **作用**：集中配置日志系统的默认级别和控制台输出级别。
    - **成员**：
        - **default_level_**：默认日志级别。
        - **console_level_**：控制台日志级别。
    - **功能**：允许设置和获取默认和控制台的日志级别。


* **🔭 服务注册配置（`service_registry_config`）**
    - **作用**： 包含服务在向注册中心注册或服务消费者连接注册中心时所需的配置信息。
    - **成员**：
        - **name_**：服务注册中心的名称。
        - **host_**：服务注册中心的主机地址。
        - **port_**：服务注册中心的端口号。


* **🩺 健康检查 URL 配置（`health_check_url_config`）**
    - **作用**： 封装了服务器健康检查`URL`的配置信息。
    - **成员**：
        - **url_**：健康检查请求的`URL`路径。
        - **interval_**：检查间隔时间。
        - **timeout_**：检查超时时间。


* **💾 数据库连接配置（`database_config`）**
    - **作用**：封装一个数据库连接所需的全部配置信息。
    - **成员**：包含数据库类型(`database_type`)、连接名、主机、端口、用户名、密码、数据库名称、连接池大小（最小/最大连接数）和超时时间。


* **🪵 日志配置（`logger_config`）**
    - **作用**：包含单个日志通道的配置信息，如文件输出、旋转大小和通道级别。
    - **成员**：包含日志通道名称、文件目录、文件名、日志级别和文件旋转大小等。

##### 聚合配置结构与顶级配置（`Aggregate and Top-Level Configurations`）

* **⚙️ 服务器配置（`server_config`）**
    - **作用**：包含一个服务器实例的所有全局配置信息。
    - **成员**：
        - **instance_id_**：服务器实例的唯一ID。
        - **service_name_**：服务名称（例如：`"auth"`, `"game"`）。
        - **server_network_config_**：容器，包含一个或多个`server_network_config`对象，定义了该服务器监听的所有网络端口和协议。
        - **game_server_id_**：游戏服ID。
        - **host_**：服务器的IP地址或主机名。
        - **worker_pool_size_**：工作池线程数量。


* **🧩 应用配置（`app_config`）**
    - **作用**：作为整个服务器应用配置的总管理器。它是所有配置类的聚合器，负责统一加载和运行时访问所有子系统配置。
    - **核心职责**：
        - **配置聚合**：内部存储`server_config`、`logger_level_config`，以及多个服务注册、数据库和日志的配置容器。
        - **配置加载**：提供一系列公共`load_xxx_config(const std::string& filename)`方法，负责从配置文件中解析并初始化所有子配置。
        - **配置访问**：提供公共`get_xxx_config()`方法，用于在程序运行时获取已加载的配置实例或配置容器。

#### worker_pool（工作池）

工作池模块是框架中负责高性能并发执行的核心组件。它的设计目标是有效管理和复用一组工作线程，以异步方式处理提交的任务，从而最大限度地利用多核处理器资源，避免频繁创建和销毁线程带来的开销。

* **💧 线程安全队列（`thread_safe_queue`）**
    - **作用**：实现一个线程安全的任务队列，用于在生产者和消费者之间安全地传递任务。
    - **特点**：使用`std::mutex`和`std::condition_variable`实现同步和阻塞等待机制。
    - **核心功能**：
        - **入队（`push`）**：将任务推入队列，并使用`notify_one()`唤醒一个等待中的工作线程。
        - **出队（`pop`）**：阻塞等待直到队列非空或队列被停止。
        - **停止（`stop`）**：设置内部标志`stop_ = true`，并使用`notify_all()`唤醒所有等待中的线程，使其安全退出。


* **💻 工作池（`worker_pool`）**
    - **作用**：实现一个高性能、多线程的任务处理工作池，用于异步执行任务，充分利用多核CPU资源。
    - **核心功能**：管理一组工作线程并提供任务提交接口。
    - **构造与销毁**：
        - **构造函数**：在创建时启动指定数量的工作线程。
        - **析构函数**：在析构时安全地停止内部的线程安全队列，并利用`noexcept_safe_call_and_log`确保停止操作的安全性。
    - **任务提交**：将一个任务`类型为std::function<void()>`提交到内部的队列中等待执行。
    - **异常处理**：工作线程内部捕获任务执行过程中抛出的`std::exception`或任何未知异常，防止异常逃逸出工作线程，并利用
      `LOG_CHANNEL`记录错误信息。

#### message（消息）

消息模块主要负责网络通信中消息的抽象、头部定义、传输所需参数的管理，以及消息处理器的抽象、具体实现和分发注册机制。

##### 基础消息结构（Basic Message Structure）

* **🏷️ 消息头部（`header`）**
    - **作用**：封装了消息的头部信息，用于标识消息的路由和内容上下文。包含RPC标识(`rpc`)、用户ID(`user_id`)和错误码 (
      `code`)。
    - **特点**：支持多种`Protobuf`消息头部类型（如`server_message_header`、`client_message_header`等）的构造。
    - **功能**：提供了`get_message()`方法，根据内部成员的值自动生成对应的`Protobuf`头部消息实例。

##### 消息处理参数封装（Message Handle Parameters）

* **📦 Protobuf消息处理参数（`protobuf_handle_parameter`）**
    - **作用**：封装处理一个`Protobuf`消息请求所需的全部上下文信息。
    - **成员**：包含消息头部(`header_`)、`Protobuf`请求消息(`request_message_`)、网络会话(`session_`)和资源加载器(
      `resource_loader_`)。使用`std::weak_ptr`管理`session` 和 `resource_loader`，避免循环引用。
    - **功能**：提供了向当前会话写回响应Protobuf消息(`write(response)`)和将请求转发给指定服务器类型(
      `write(server_type, request)`)的方法。


* **📦 HTTP消息处理参数（`http_handle_parameter`）**
    - **作用**：封装处理`HTTP`请求所需的全部上下文信息，是`HTTP`消息处理的参数载体。
    - **成员**：包含`io_context`、请求路径 (`path`)、参数(`params`)、会话(`session`) 和资源加载器(`resource_loader`)。
    - **功能**：提供了向当前会话写回`HTTP`响应字符串(`write(response)`)、获取请求路径和获取应用配置等方法。

##### 抽象处理器基类（Abstract Handler Base Classes）

* **⚙️ Protobuf消息处理器基类（`protobuf_base_message_handler`）**
    - **作用**：定义所有`Protobuf`消息处理器的抽象基类。
    - **接口**：定义了处理器必须实现的纯虚函数：`get_supported_type_name()`（获取支持的消息类型名称）和`handle(...)`
      （核心处理逻辑）。


* **🎯 Protobuf消息处理器（`concrete_message_handler<Message>`）**
    - **作用**：`Protobuf`消息处理器的模板实现基类，继承自 `protobuf_base_message_handler`。旨在简化具体业务处理器的实现。
    - **功能**：
        - 实现`handle()` 接口，将通用`protobuf_message` 安全地**向下转型**为具体的`Message` 类型，并调用子类实现的
          `handle_concrete()`。
        - 支持根据`Protobuf` `oneof` 字段的 `payload_case` 注册和查找具体的子处理函数。
        - 提供消息转发 (`handle_forward`) 和嵌套消息分发 (`handle_dispatch`) 等高级处理逻辑。


* **⚙️ HTTP消息处理器基类（`http_base_message_handler`）**
    - **作用**：定义所有`HTTP`消息处理器的抽象基类。
    - **接口**：定义了处理器必须实现的纯虚函数：`get_supported_type_name()`（获取支持的 URL 路径）和`handle(...)`（核心处理逻辑）。

##### 消息分发注册机制（Message Dispatch and Registry）

* **🔄 Protobuf消息注册与分发（`protobuf_message_registry`）**
    - **作用**：负责集中注册和运行时查找`Protobuf`消息处理器，并将收到的消息分发给对应的处理器实例。
    - **核心功能**：**注册 (`registerHandler`)** 和 **分发 (`dispatch`)**。
    - **线程安全**：使用`std::shared_mutex`保护内部注册表，确保注册操作的线程安全。


* **🔄 HTTP消息注册与分发（`http_message_registry`）**
    - **作用**：负责集中注册和运行时查找`HTTP`消息处理器，并将收到的`HTTP`请求分发给对应的处理器实例。
    - **核心功能**：**注册 (`registerHandler`)**（以 URL 路径/类型名为键）和 **分发 (`dispatch`)**。
    - **线程安全**：使用`std::shared_mutex`保护内部注册表，确保注册操作的线程安全。

#### database（数据库）

数据库模块提供了一个统一的、与具体数据库类型无关的接口，用于执行数据持久化操作。它支持多种数据库（MySQL, MongoDB,
Redis），并提供了连接池管理、数据抽象和命令封装等功能。

##### 基础定义与数据表示 (Basic Definitions & Data Representation)

* **📊 数据库基础类型 (`database_data_type`, `database_index_type`, `database_change_type`)**
    - **作用**：这组枚举类型是数据库模块的基石，分别定义了支持的数据类型（如`string`, `int32`）、字段的索引类型（如主键、唯一键）和数据库操作的类型（如
      `insert`, `update`, `delete`）。
    - **特点**：提供了类型安全的常量，避免了使用魔法字符串或数字，增强了代码的可读性和可维护性。


* **🔬 数据库类型特征 (`database_data_Type_traits<Type>`)**
    - **作用**：一个模板元编程工具，用于在编译时获取 `database_data_type` 枚举值与实际 C++ 类型之间的映射关系。
    - **特点**：通过模板特化，为每个 `database_data_type` 值定义了对应的 `Type`（例如，`database_data_type::string_type` 对应
      `std::string`），是实现类型安全数据访问的关键。


* **🏷️ 数据库字段 (`database_field`)**
    - **作用**：表示数据库表的单个字段的元数据。
    - **特点**：封装了字段的名称、数据类型 (`database_data_type`) 和索引类型 (`database_index_type`)。它是定义
      `database_entity` 的基本组成部分。


* **🧱 基础数据单元 (`basis_database`)**
    - **作用**：框架中数据交换的核心单元，用于封装单个字段的数据。它是一个与具体数据库无关的通用数据容器。
    - **特点**：内部使用 `std::any` 存储不同类型的值，并记录字段名和数据类型 (`database_data_type`)。提供了
      `get_value<T>()`、`get_string()` 等方法来安全地访问数据。


* **📦 数据容器 (`basis_database_container`)**
    - **作用**：作为 `basis_database` 对象的容器，通常用于表示数据库中的一行记录或一个文档。
    - **特点**：本质上是一个 `std::vector<basis_database>`，提供了一种标准方式来组织和传递多个字段的数据。


* **⚙️ 数据管理器 (`basis_database_manager`)**
    - **作用**：一个高级数据管理器，用于封装和操作 `basis_database_container`。它不仅存储数据，还能跟踪数据的变更状态。
    - **功能**:
        - 存储主键 (`key`) 和数据 (`database`)。
        - 记录数据的变更类型 (`database_change_type`)，如 `insert_type`, `update_type`, `delete_type`。
        - 提供了 `modify()` 方法来更新字段值。

##### 实体 (Entity)

* **🧬 编译时实体 (`entity<...>`)**
    - **作用**：一个编译时模板元编程工具，用于以声明方式定义数据库表的结构。
    - **特点**：通过模板参数接收字段名、数据类型和索引类型，从而在编译时生成表的元数据。


* **🏛️ 运行时实体 (`database_entity`)**
    - **作用**：数据库表的运行时表示。它由 `entity` 定义生成，用于在程序运行时执行实际的数据库操作。
    - **功能**:
        - 提供了 `select_one()`, `select_all()`, `update()`, `insert()`, `delete()` 等高级数据操作接口。
        - 内部封装了与 `database_pool_manager` 的交互，自动处理数据库会话的获取和释放。

##### 会话 (Session)

* **🌐 数据库会话基类 (`database_session`)**
    - **作用**：定义了数据库会话的抽象接口，是所有具体数据库会话（如 MySQL, MongoDB, Redis）的基类。
    - **接口**：提供了 `select_one()`, `select_all()`, `execute_changes()` 等纯虚函数，强制子类实现标准的数据库操作。


* **🗄️ MySQL数据库会话 (`mysql_database_session`)**
    - **作用**：`database_session` 针对 MySQL 的具体实现。
    - **特点**：封装了 `boost::mysql` 库的 API 调用细节，将底层的 MySQL 操作适配到统一的 `database_session` 接口。


* **📄 MongoDB数据库会话 (`mongo_database_session`)**
    - **作用**：`database_session` 针对 MongoDB 的具体实现。
    - **特点**：封装了 `mongocxx` 驱动的 API 调用细节，将底层的文档操作适配到统一的 `database_session` 接口。


* **⚡ Redis数据库会话 (`redis_database_session`)**
    - **作用**：`database_session` 针对 Redis 的具体实现。
    - **特点**：封装了 `hiredis` 库的 API 调用细节，提供了执行原生 Redis 命令的能力，并作为所有 `redis_*_commands` 类的基础。

##### 连接池 (Connection Pool)

* **💧 连接池基类 (`connection_pool_base<SessionType>`)**
    - **作用**：一个通用的、基于模板的数据库连接池基类。
    - **功能**：实现了连接池的核心逻辑，包括异步获取 (`acquire`)、归还 (`release`) 连接，以及定时清理 (`cleanup`) 空闲连接。


* **🏊 数据库连接池 (`database_pool`)**
    - **作用**：`connection_pool_base` 的具体实现，用于管理特定类型 `database_session` 的连接池。
    - **特点**：封装了创建和管理特定数据库会话（如 `mysql_database_session`）的逻辑。


* **👨‍💼 数据库连接池管理器 (`database_pool_manager`)**
    - **作用**：一个单例管理器，负责创建、存储和提供对项目中所有 `database_pool` 的全局访问。
    - **核心功能**:
        - 在服务器启动时，根据配置初始化所有需要的数据库连接池。
        - 提供 `get_session<SessionType>()` 接口，允许业务代码方便地从指定的连接池中获取一个数据库会话。

##### Redis 命令封装 (Redis Command Wrappers)

* **📜 Redis命令基类 (`redis_commands`)**
    - **作用**：所有`Redis`命令封装类的基类。
    - **特点**：持有一个 `redis_database_session` 的共享指针，所有子类命令都通过这个会话执行。


* **🔑 Redis键命令 (`redis_key_commands`)**
    - **作用**：封装与`Redis`键（`Key`）相关的操作。
    - **功能**：提供类型安全的函数来执行 `DEL`, `EXISTS`, `EXPIRE`, `TTL` 等命令，管理键的生命周期和存在性。


* **📝 Redis字符串命令 (`redis_string_commands`)**
    - **作用**：封装与`Redis`字符串（`String`）相关的操作。
    - **功能**：提供类型安全的函数来执行 `GET`, `SET`, `INCR`, `DECR` 等命令，用于基本的键值存储。


* **🗂️ Redis哈希命令 (`redis_hash_commands`)**
    - **作用**：封装与`Redis`哈希（`Hash`）相关的操作。
    - **功能**：提供类型安全的函数来执行 `HGET`, `HSET`, `HGETALL`, `HDEL` 等命令，适用于存储对象结构。


* **📋 Redis列表命令 (`redis_list_commands`)**
    - **作用**：封装与`Redis`列表（`List`）相关的操作。
    - **功能**：提供类型安全的函数来执行 `LPUSH`, `RPOP`, `LRANGE`, `LLEN` 等命令，适用于实现队列或时间线等功能。


* **🧩 Redis集合命令 (`redis_set_commands`)**
    - **作用**：封装与`Redis`集合（`Set`）相关的操作。
    - **功能**：提供类型安全的函数来执行 `SADD`, `SREM`, `SMEMBERS`, `SISMEMBER` 等命令，用于存储无序且唯一的元素集合。


* **📈 Redis有序集合命令 (`redis_sorted_set_commands`)**
    - **作用**：封装与`Redis`有序集合（`Sorted Set`）相关的操作。
    - **功能**：提供类型安全的函数来执行 `ZADD`, `ZREM`, `ZRANGE`, `ZSCORE` 等命令，适用于排行榜、优先级队列等场景。


* **⚖️ 有序集合成员与分数 (`sorted_set_member_score`)**
    - **作用**：一个简单的数据结构，用于表示`Redis`有序集合（`Sorted Set`）中一个成员及其对应的分数。


* **🔍 扫描结果 (`scan_result`)**
    - **作用**：封装了`Redis`的`SCAN`迭代命令的返回结果。
    - **特点**：包含下一次迭代所需的游标 (`cursor`) 和当前批次获取的数据 (`data`)。

#### network（网络）

#### service_registry（服务注册）

#### handler（处理器）

#### initializer（初始化）

### assist（辅助）

#### generate_handler_tools（生成处理器工具）

#### generate_database_tools（生成数据库工具）

### server（服务器）

#### service registry（服务注册中心）

服务注册中心是微服务架构的核心组件，它作为所有服务的“通讯录”，负责服务的注册、发现和健康检查，是实现服务间动态通信和高可用的基础。

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

#### auth（认证服务器）

认证服务器是整个系统的安全入口，负责验证用户身份、管理账户信息，并生成和验证访问凭证（Token）。

- 🔐 用户注册与登录 (User Registration & Login)（未实现）
    - 提供新用户注册接口，支持用户名/密码、第三方平台（如微信、QQ）等多种注册方式。
    - 处理用户登录请求，验证凭据（如密码哈希比对）。
    - 实现账户锁定、防暴力破解等安全机制。


- 🎫 凭证生成与验证 (Token Generation & Validation)（未实现）
    - 登录成功后，生成一个有时效性的、包含用户身份信息的安全令牌（如 JWT, JSON Web Token）。
    - 提供对外的令牌验证接口，供其他业务服务器（如网关、游戏服务器）调用，以验证用户请求的合法性。
    - 管理令牌的生命周期，支持令牌刷新（Refresh Token）和强制失效（注销）。


- 📇 账户信息管理 (Account Information Management)（未实现）
    - 存储和管理用户账户核心数据，如账户名、密码哈希、用户ID、创建时间等。
    - 与数据库交互，持久化账户信息。


- 🔗 第三方平台集成 (Third-Party Integration)（未实现）
    - 处理来自第三方平台（如微信、Apple Game Center）的登录回调。
    - 根据第三方平台提供的用户信息，创建或关联本地账户。

#### payment（支付服务器）

支付服务器的核心职责是处理游戏内的所有支付请求，确保交易的安全、准确和可靠。

- 💰 支付请求处理 (Payment Request Processing)（未实现）
    - 接收来自客户端或游戏服务器的支付请求。
    - 验证支付请求的合法性，包括商品信息、金额、用户身份等。


- 🤝 第三方支付平台集成 (Third-Party Payment Platform Integration)（未实现）
    - 对接各种第三方支付平台（如微信支付、支付宝、Apple Pay、Google Play Billing等）。
    - 处理支付回调通知，更新订单状态。


- 🛡️ 交易安全与防作弊 (Transaction Security & Anti-Cheating)（未实现）
    - 实现交易加密、签名验证等安全措施，防止支付信息被篡改。
    - 建立防作弊机制，检测和阻止恶意支付行为。


- 📜 订单管理 (Order Management)（未实现）
    - 创建、查询、更新和管理支付订单。
    - 记录订单的详细信息，包括商品、金额、用户、支付状态、交易时间等。


- 🔄 支付结果通知 (Payment Result Notification)（未实现）
    - 将支付结果通知给相关的游戏服务器或客户端。
    - 处理支付失败、退款等异常情况。

#### admin（后台服务器）

后台服务器（Admin Server）为游戏运营和管理人员提供强大的工具，用于管理游戏数据、用户账户、系统配置和运营活动，确保游戏的正常运行和健康发展。

- ⚙️ 游戏数据管理 (Game Data Management)（未实现）
    - 提供对游戏内各种数据（如物品、角色属性、任务、NPC等）的查询、修改、添加和删除功能。
    - 支持批量操作和数据导入/导出。


- 👥 用户账户管理 (User Account Management)（未实现）
    - 查询用户基本信息、游戏数据、充值记录等。
    - 支持封禁/解封用户、修改用户密码、重置角色数据等操作。
    - 提供用户行为日志查询，用于问题排查和作弊检测。


- 📊 运营活动管理 (Operational Activity Management)（未实现）
    - 配置和发布游戏内活动（如充值活动、节日活动、限时副本）。
    - 实时监控活动数据和效果。


- 📈 数据统计与分析 (Data Statistics & Analysis)（未实现）
    - 提供关键运营数据（DAU, MAU, ARPU, LTV等）的实时监控和历史查询。
    - 生成各种报表，辅助运营决策。


- 📢 公告与邮件系统 (Announcement & Mail System)（未实现）
    - 发布游戏内公告、维护通知。
    - 发送全服或指定用户的游戏内邮件，支持附件（如道具、货币）。


- 🛡️ 权限管理 (Permission Management)（未实现）
    - 管理后台用户的角色和权限，确保不同运营人员只能访问其职责范围内的功能。
    - 记录后台操作日志，方便审计和追溯。


- 🛠️ 系统配置管理 (System Configuration Management)（未实现）
    - 动态修改游戏服务器的配置参数，无需重启服务器。
    - 管理游戏版本、资源更新等。

#### chat（聊天服务器）

聊天服务器负责处理游戏内所有玩家之间的实时通讯，包括私聊、群聊、频道聊天等，确保消息的及时传递和良好的社交体验。

- 💬 实时消息传递 (Real-time Message Delivery)（未实现）
    - 支持点对点私聊功能。
    - 支持群组聊天和自定义频道聊天。
    - 支持世界频道、公会频道等公共聊天区域。


- 📥 消息存储与离线消息 (Message Storage & Offline Messages)（未实现）
    - 存储聊天记录，方便用户查询历史消息。
    - 实现离线消息机制，确保用户上线后能接收到离线期间的消息。


- 🚫 消息过滤与敏感词 (Message Filtering & Sensitive Words)（未实现）
    - 实时过滤聊天内容中的敏感词汇和不当信息。
    - 支持消息审核机制，对违规内容进行处理。


- 😊 表情与富文本 (Emoticons & Rich Text)（未实现）
    - 支持在聊天中使用表情符号。
    - 支持简单的富文本格式，如颜色、加粗等。


- 📢 频道管理 (Channel Management)（未实现）
    - 允许用户创建、加入和退出聊天频道。
    - 提供频道权限管理功能，如禁言、踢人、设置管理员等。


- 👤 用户状态 (User Status)（未实现）
    - 显示玩家的在线/离线状态。
    - 支持显示玩家“正在输入”的状态提示。

#### gateway（网关服务器）

网关服务器是客户端连接游戏服务器集群的唯一入口，负责客户端连接的接入、认证、消息转发和负载均衡等核心功能。

- 🔗 客户端连接管理 (Client Connection Management)（未实现）
    - 负责维护客户端与服务器之间的 TCP/WebSocket 等长连接。
    - 处理连接的建立、断开和心跳检测。


- 🔐 身份认证与授权 (Authentication & Authorization)（未实现）
    - 接收客户端的登录请求，并与认证服务器交互，验证用户身份。
    - 验证通过后，为客户端生成会话令牌，并管理会话状态。


- 🔄 消息路由与转发 (Message Routing & Forwarding)（未实现）
    - 根据消息类型和目标，将客户端请求转发到相应的后端业务服务器（如玩家服务器、逻辑服务器）。
    - 将后端服务器的响应消息转发回对应的客户端。


- ⚖️ 负载均衡 (Load Balancing)（未实现）
    - 根据后端服务器的负载情况，将新的客户端连接分配到最合适的业务服务器实例。
    - 支持多种负载均衡策略（如轮询、最少连接、哈希等）。

- 🛡️ 安全防护 (Security Protection)（未实现）
    - 实现基本的 DDoS 防护、流量清洗。
    - 过滤恶意请求和非法数据包。


- 📈 流量控制与限流 (Traffic Control & Rate Limiting)（未实现）
    - 限制单个客户端或总体的请求速率，防止服务器过载。
    - 支持动态调整限流策略。


- 📊 状态监控与日志 (Status Monitoring & Logging)（未实现）
    - 实时监控网关的连接数、流量、错误率等关键指标。
    - 记录详细的连接和消息日志，方便故障排查和分析。

#### player（玩家服务器）

玩家服务器（Player Server）是游戏中负责管理玩家核心数据和状态的关键组件。它主要关注玩家的持久化身份、资产和非实时游戏逻辑相关的状态，是玩家在游戏世界中的“身份档案”和“个人仓库”。

- 👤 玩家数据管理 (Player Data Management)（未实现）
    - 存储和管理玩家的角色基本信息（名称、等级、职业、外观等）。
    - 存储和管理玩家的虚拟资产（货币、道具、装备、背包）。
    - 存储和管理玩家的成就、任务进度、邮件、日志等非实时数据。
    - 与数据库进行交互，确保持久化存储和数据一致性。


- 🌐 玩家状态管理 (Player State Management)（未实现）
    - 维护玩家的在线/离线状态。
    - 记录玩家当前所在的游戏世界、场景或逻辑服务器实例。
    - 管理玩家的会话信息，确保玩家在不同游戏组件间的无缝切换。


- 🤝 社交关系管理 (Social Relationship Management)（未实现）
    - 处理好友列表、黑名单的添加、删除和查询。
    - 管理公会/帮派信息（如果公会不是一个独立的微服务）。
    - 提供玩家间的邮件系统。


- 🎭 角色创建与选择 (Character Creation & Selection)（未实现）
    - 处理玩家创建新角色、删除角色、选择角色进入游戏等操作。
    - 验证角色名称的合法性和唯一性。


- 🔄 数据同步与一致性 (Data Synchronization & Consistency)（未实现）
    - 确保玩家数据在不同游戏服务器（如逻辑服务器、战斗服务器）之间进行同步。
    - 处理玩家数据在服务器重启、宕机等情况下的恢复和一致性。


- 💰 资源管理 (Resource Management)（未实现）
    - 处理玩家拥有的虚拟货币、道具等资源的增减操作。
    - 确保资源操作的原子性和安全性，防止数据异常。

#### logic（逻辑服务器）

逻辑服务器是处理大部分非实时、但与核心玩法紧密相关的游戏逻辑的“大脑”。它负责驱动玩家在游戏世界中的各种行为和状态变迁，是游戏核心玩法的实现者。

- 任务系统 (Quest System)（未实现）
    - 处理任务的接取、完成、放弃、进度更新等逻辑。
    - 管理任务链和任务奖励发放。


- 🗺️ 场景与世界管理 (Scene & World Management)（未实现）
    - 管理玩家在不同场景（地图）之间的切换逻辑。
    - 控制场景中非战斗相关的动态元素，如NPC的刷新、可采集物的状态等。
    - 驱动游戏世界中的定时事件或触发式事件。


- 💬 NPC交互 (NPC Interaction)（未实现）
    - 处理与NPC的对话、商店买卖、任务交付等交互逻辑。
    - 管理NPC的行为和状态。


- 🛠️ 玩家非战斗行为 (Player Non-Combat Actions)（未实现）
    - 处理采集、合成、制造、装备强化、技能升级等非战斗核心循环逻辑。
    - 验证玩家操作的合法性，并更新玩家数据。


- 🔄 服务间协调 (Inter-Service Coordination)（未实现）
    - 接收来自网关的玩家操作请求，并进行初步处理。
    - 与玩家服务器通信，获取和更新玩家的持久化数据（如扣除任务道具、增加经验值）。
    - 与游戏服务器（战斗服务器）通信，在进入/退出战斗时同步玩家状态。
    - 与聊天服务器等其他服务交互，以广播游戏事件（如玩家完成了某个重要成就）。


- 📊 状态同步与广播 (State Synchronization & Broadcast)（未实现）
    - 将游戏逻辑处理的结果同步给客户端，以更新UI和玩家视角内的世界状态。
    - 确保玩家的逻辑状态在各个后端服务之间保持一致。

#### game（游戏服务器）

游戏服务器（Game Server）负责承载和执行某个特定游戏服（Shard/Instance）的核心、独特游戏逻辑和功能。它管理该服内的游戏世界状态、玩家在该服内的具体行为，并驱动游戏核心玩法的运行。

- 🎮 核心游戏玩法逻辑 (Core Gameplay Logic)（未实现）
    - 实现游戏独有的规则、机制和系统，如独特的经济系统、复杂的制作系统、特定的任务链或世界事件。
    - 处理玩家在游戏世界中与游戏特定对象、系统或机制的交互。


- 🗺️ 游戏世界/实例状态管理 (Game World/Instance State Management)（未实现）
    - 维护特定游戏服内游戏世界的实时状态，包括地图、区域、副本或战场实例。
    - 管理该服内所有NPC、怪物、可交互物品和环境元素的生命周期和状态。


- 🏃 玩家服内行为处理 (Player In-Shard Action Processing)（未实现）
    - 处理玩家在该游戏服内的各种操作，例如交易、与游戏对象互动、参与游戏服特有的活动或事件。
    - 验证玩家操作的合法性，并根据游戏规则更新游戏世界和玩家状态。


- 🔄 游戏数据持久化与同步 (Game Data Persistence & Synchronization)（未实现）
    - 与玩家服务器通信，加载玩家进入该服所需的角色数据，并在玩家离开或数据变更时同步回玩家服务器。
    - 持久化该游戏服特有的世界状态数据（如副本进度、世界Boss状态）到数据库。


- 📢 游戏事件管理与广播 (Game Event Management & Broadcasting)（未实现）
    - 触发和管理游戏服内的各种事件，如定时活动、Boss刷新、天气变化等。
    - 向该服内所有相关玩家广播游戏事件通知和状态更新。

#### cross（跨服服务器）

跨服服务器（Cross
Server）是游戏架构中用于连接不同游戏服务器（Shard/Instance）玩家的桥梁。它使得来自不同服务器的玩家能够参与到共同的活动、竞技或社交互动中，打破了单一服务器的界限，扩展了游戏世界的规模和玩家的互动范围。

- 🌐 跨服活动管理 (Cross-Server Activity Management)（未实现）
    - 组织和管理跨服活动，如跨服战场、跨服竞技场、跨服副本等。
    - 负责跨服活动的匹配、房间创建和状态管理。


- 🤝 跨服玩家匹配 (Cross-Server Player Matching)（未实现）
    - 根据玩家的等级、战斗力、段位等信息，进行跨服匹配。
    - 支持多种匹配算法，确保匹配的公平性和效率。


- 🔄 跨服数据同步 (Cross-Server Data Synchronization)（未实现）
    - 在跨服活动期间，同步来自不同服务器的玩家数据。
    - 确保跨服活动结束后，玩家的奖励和数据能够正确地写回到各自的源服务器。


- 💬 跨服聊天 (Cross-Server Chat)（未实现）
    - 提供跨服聊天频道，让不同服务器的玩家可以进行实时交流。


- 🏆 跨服排行榜 (Cross-Server Leaderboards)（未实现）
    - 实现跨服排行榜功能，展示所有服务器玩家的排名。
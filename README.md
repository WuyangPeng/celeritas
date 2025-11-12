# celeritas

本设计旨在为一款高性能游戏服务器提供一个基础框架。该框架将使用C++20标准和boost库，实现一个异步、多线程、高吞吐量的系统。

## Supported Platforms（支持平台）：

- windows
- linux

## Supported Compiler（支持编译器）：

- MinGW 15.2 (windows)
- Visual Studio 2022  (windows)
- gcc 14.2  (linux)

## Dependency Library（依赖库）：

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

- tools 工具

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
    - **功能**：提供了全局函数`get_server_network_type(const std：：string&)`，用于将协议名称字符串转换为对应的枚举值。


* **📊 数据库类型（`database_type`）**
    - **作用**：一个枚举类型，用于明确表示框架支持的数据库类型。它是数据库配置的基础。
    - **枚举值**：`unknown`、`mysql`、`mongodb`、`redis`。
    - **功能**：提供了全局函数`get_database_type(const std：：string&)`，用于将数据库类型名称字符串转换为对应的枚举值。

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
        - **service_name_**：服务名称（例如: `"auth"`, `"game"`）。
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

#### database（数据库）

#### network（网络）

#### service_registry（服务注册）

#### handler（处理器）

#### initializer（初始化）

### assist（辅助）

#### tools（工具）

### server（服务器）

#### service registry（服务注册中心）

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

#### payment（支付服务器）

#### admin（后台服务器）

#### chat（聊天服务器）

#### gateway（网关服务器）

#### player（玩家服务器）

#### logic（逻辑服务器）

#### game（游戏服务器）

#### cross（跨服服务器）
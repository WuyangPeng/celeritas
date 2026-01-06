# database（数据库）

数据库模块提供了一个统一的、与具体数据库类型无关的接口，用于执行数据持久化操作。它支持多种数据库（MySQL, MongoDB,
Redis），并提供了连接池管理、数据抽象和命令封装等功能。

## basic definitions & data representation (基础定义与数据表示)

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


* **⚙️ 数据实体变更 (`database_entity_change`)**
    - **作用**：一个高级数据管理器，用于封装和操作 `basis_database_container`。它不仅存储数据，还能跟踪数据的变更状态。
    - **功能**:
        - 存储主键 (`key`) 和数据 (`database`)。
        - 记录数据的变更类型 (`database_change_type`)，如 `insert_type`, `update_type`, `delete_type`。
        - 提供了 `modify()` 方法来更新字段值。

## entity (实体)

* **🧬 编译时实体 (`entity<...>`)**
    - **作用**：一个编译时模板元编程工具，用于以声明方式定义数据库表的结构。
    - **特点**：通过模板参数接收字段名、数据类型和索引类型，从而在编译时生成表的元数据。


* **🏛️ 运行时实体 (`database_entity`)**
    - **作用**：数据库表的运行时表示。它由 `entity` 定义生成，用于在程序运行时执行实际的数据库操作。
    - **功能**:
        - 提供了 `get_modify()`, `get_delete()`等数据操作接口。
        - 内部封装了与 `database_entity_change` 的交互。

## session (会话)

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

## connection pool (连接池)

* **🏊 数据库连接池基类 (`database_pool_base`)**
    - **作用**：定义了数据库连接池的抽象接口，是所有具体连接池实现（如 `connection_pool<SessionType>`）的基类。
    - **特点**：提供了执行数据库操作（如查询、修改）和管理连接池（如健康检查）的统一接口，实现了业务逻辑与具体连接池实现的解耦。


* **💧 数据库连接池 (`connection_pool<SessionType>`)**
    - **作用**：一个通用的、基于模板的数据库连接池。
    - **功能**：实现了连接池的核心逻辑，包括异步获取 (`acquire`)、归还 (`release`) 连接，以及定时清理 (`cleanup`) 空闲连接。


* **👨‍💼 数据库连接池管理器 (`database_pool_manager`)**
    - **作用**：一个单例管理器，负责创建、存储和提供对项目中所有 `database_pool` 的全局访问。
    - **核心功能**:
        - 在服务器启动时，根据配置初始化所有需要的数据库连接池。
        - 提供 `get_session<SessionType>()` 接口，允许业务代码方便地从指定的连接池中获取一个数据库会话。


* **🔒 数据库会话 RAII 守卫 (`database_session_guard<SessionType>`)**
    - **作用**：遵循 RAII 原则，用于**安全管理**从`connection_pool`中获取的数据库会话。
    - **安全**：在析构时**自动**将其归还 (`release_session`) 到 `connection_pool`，其析构函数标记为`noexcept`，并依赖
      `noexcept_safe_call_and_log`确保归还操作的安全性。
    - **用法**：通过 `database_pool_manager::get_session<SessionType>()` 获取会话时，返回的即是 `database_session_guard`
      实例。

## 配置管理 (Configuration Management)

* **⚙️ 配置管理器 (`config_manager`)**
    - **作用**：作为单例类，负责从数据库中异步加载和缓存所有配置信息（如`time_refresh`）。
    - **特点**：
        - **配置加载与缓存**：在服务启动时加载所有配置，并缓存在内存中。
        - **线程安全访问**：提供线程安全的方法来获取指定配置信息。
        - **热重载**：支持通过 `reload_from_db` 方法按需重新加载配置，无需重启服务。

## redis command wrappers（redis命令封装）

* **📜 Redis命令基类 (`redis_commands`)**
    - **作用**：所有`Redis`命令封装类的基类。
    - **特点**：持有一个 `redis_database_session` 的引用，所有子类命令都通过这个会话执行。


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

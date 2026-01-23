# message（消息）

消息模块主要负责网络通信中消息的抽象、头部定义、传输所需参数的管理，以及消息处理器的抽象、具体实现和分发注册机制。

## basic message structure（基础消息结构）

* **🎮 游戏错误码 (`game_error_type`)**
    - **作用**：定义了游戏业务逻辑中的通用错误码。
    - **特点**：使用`enum class`提供了类型安全的错误码，便于在不同服务之间传递和识别具体的业务错误。


* **🏷️ 消息头部（`header`）**
    - **作用**：封装了消息的头部信息，用于标识消息的路由和内容上下文。包含RPC标识(`rpc`)、用户ID(`user_id`)和错误码 (
      `code`)。
    - **特点**：支持多种`Protobuf`消息头部类型（如`server_message_header`、`client_message_header`等）的构造。
    - **功能**：提供了`get_message()`方法，根据内部成员的值自动生成对应的`Protobuf`头部消息实例。


* **❌ 游戏业务异常 (`celeritas_game_error`)**
    - **作用**：框架中用于表示游戏业务逻辑错误的自定义异常类。
    - **特点**：继承自`celeritas_error`，并额外携带一个 `game_error_type` 错误码，使得异常信息更加具体，方便上层逻辑根据错误码进行处理。


* **📄 HTTP响应 (`http_response`)**
    - **作用**：封装一个包含游戏业务错误码和描述信息的`HTTP`响应体。
    - **功能**：提供`to_json_string()`方法，将内部的`game_error_type`错误码和`message`序列化为`JSON`字符串，作为`HTTP`
      响应的内容。

## message handle parameters（消息处理参数封装）

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

## abstract handler base classes（抽象处理器基类）

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


* **🌐 HTTP服务基类（`http_service_base`）**
    - **作用**：定义了处理具体`HTTP`业务逻辑的服务基类。
    - **接口**：
        - `response()`：纯虚函数，子类需实现具体的业务逻辑并生成响应。
        - `send_error_response()`：纯虚函数，子类需实现错误响应的发送逻辑。
    - **功能**：
        - 提供了`write_immediately(response)`方法，用于立即发送`HTTP`响应。
        - 提供了获取请求参数(`get_param`)、应用配置(`get_app_config`)和提交异步任务(`submit_task`)等辅助方法。
        - 管理`http_handle_parameter`，方便子类访问请求上下文。

## message dispatch and registry（消息分发注册机制）

* **🔄 Protobuf消息注册与分发（`protobuf_message_registry`）**
    - **作用**：负责集中注册和运行时查找`Protobuf`消息处理器，并将收到的消息分发给对应的处理器实例。
    - **核心功能**：**注册 (`registerHandler`)** 和 **分发 (`dispatch`)**。
    - **线程安全**：使用`std::shared_mutex`保护内部注册表，确保注册操作的线程安全。


* **🔄 HTTP消息注册与分发（`http_message_registry`）**
    - **作用**：负责集中注册和运行时查找`HTTP`消息处理器，并将收到的`HTTP`请求分发给对应的处理器实例。
    - **核心功能**：**注册 (`registerHandler`)**（以 URL 路径/类型名为键）和 **分发 (`dispatch`)**。
    - **线程安全**：使用`std::shared_mutex`保护内部注册表，确保注册操作的线程安全。

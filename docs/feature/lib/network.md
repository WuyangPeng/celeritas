# network（网络）

网络模块提供了高性能、可扩展的异步网络通信框架，支持多种协议（`TCP`, `HTTP`, `WebSocket`），并提供了连接管理、消息处理和事件回调等功能。

## core abstractions（核心抽象）

* **🌐 会话基类 (`session_base`)**
    - **作用**：定义了所有具体网络会话的抽象接口。
    - **特点**：作为`generic_session`等具体会话实现的基类，提供了统一的会话管理和生命周期控制。


* **👂 监听器基类 (`listener`)**
    - **作用**：定义了所有网络监听器的抽象接口。
    - **特点**：作为 `tcp_listener`, `http_listener`, `websocket_listener` 等具体监听器实现的基类。


* **✉️ 网络消息回调 (`network_message_callback`)**
    - **作用**：处理网络接收到的具体消息的回调接口。
    - **特点**：将原始网络数据转换为可处理的消息格式，并分发给相应的消息处理器。


* **🏷️ 消息头部 (`message_header`)**
    - **作用**：定义网络消息的头部结构。
    - **特点**：包含消息的元数据，如消息类型、长度、`RPC`、`ID`等，用于消息的解析和路由。

## session implementations（会话实现）

* **🔗 通用会话 (`generic_session<SocketType>`)**
    - **作用**：一个模板化的会话基类，用于处理底层`Socket`类型的通用网络操作。
    - **特点**：封装了异步读写操作，通过模板参数`SocketType`适配不同类型的网络连接（如 `boost::asio::ip::tcp::socket`）。


* **📄 HTTP会话 (`http_session`)**
    - **作用**：处理`HTTP`协议的会话。
    - **特点**：专注于`HTTP`请求和响应的解析与构建。


* **🕸️ WebSocket会话 (`websocket_session`)**
    - **作用**：处理`WebSocket`协议的会话。
    - **特点**：专注于`WebSocket`帧的解析、构建和握手过程。

## session helper components（会话辅助组件）

* **🏃 会话运行器 (`session_run`)**
    - **作用**：管理会话的异步读写循环，驱动会话的持续运行。
    - **特点**：通常与`generic_session`配合使用，处理数据的接收和发送，并将数据传递给消息处理器。


* **✍️ 会话写入器 (`session_write`)**
    - **作用**：负责会话的异步数据写入操作。
    - **特点**：提供统一的接口来发送数据，处理写入队列和错误。


* **📞 会话回调 (`session_callback`)**
    - **作用**：定义会话事件的回调接口。
    - **特点**：允许业务逻辑订阅会话的连接建立、数据接收、连接关闭等事件。

## listener implementations（监听器实现）

* **🤝 监听器连接接受器 (`listener_accept`)**
    - **作用**：负责异步接受新的客户端连接。
    - **特点**：封装了`boost::asio::async_accept`等操作，并将新连接传递给会话管理器。


* **👥 监听器会话管理基类 (`listener_sessions_base`)**
    - **作用**：定义了监听器管理其所有活跃会话的抽象接口。
    - **特点**：提供了会话的注册、查找和移除等功能。


* **📈 监听器会话管理器 (`listener_sessions`)**
    - **作用**：管理由监听器接受的所有活跃网络会话。
    - **特点**：维护会话列表，处理会话的生命周期，并支持会话的查找和遍历。


* **🔌 TCP监听器 (`tcp_listener`)**
    - **作用**：监听并接受`TCP`客户端连接。
    - **特点**：基于`listener`实现，专注于`TCP`协议的连接管理。


* **🌐 HTTP监听器 (`http_listener`)**
    - **作用**：监听并接受 HTTP 客户端连接。
    - **特点**：基于`listener`实现，专注于`HTTP`协议的连接管理和请求分发。


* **🕸️ WebSocket监听器 (`websocket_listener`)**
    - **作用**：监听并接受 WebSocket 客户端连接。
    - **特点**：基于`listener`实现，专注于`WebSocket`协议的连接管理和握手过程。

## client implementations（客户端实现）

* **➡️ TCP客户端 (`tcp_client`)**
    - **作用**：用于主动发起`TCP`连接并进行通信。
    - **特点**：封装了`TCP`连接的建立和数据传输。


* **↗️ HTTP客户端 (`http_client`)**
    - **作用**：用于主动发起`HTTP`请求并接收响应。
    - **特点**：封装了`HTTP`请求的构建和发送，以及响应的解析。

# handler（处理器）

处理器模块定义了消息处理器的抽象接口和注册机制，用于接收和处理来自网络层的各种消息（如`Protobuf`消息、`HTTP`
请求）。它将消息分发给对应的业务逻辑处理器，实现业务逻辑与网络协议的解耦。

## service registry & discovery (服务注册与发现)

* **🔍 发现请求消息处理器 (`discover_request_message_handler`)**
    - **作用**：处理客户端或服务发起的发现服务请求。
    - **功能**：接收并处理`discover_request`消息，返回可用服务实例列表。


* **📢 发现响应消息处理器 (`discover_response_message_handler`)**
    - **作用**：处理服务发现请求的响应。
    - **功能**：接收并处理`discover_response`消息，更新本地服务列表。


* **✍️ 注册请求消息处理器 (`register_request_message_handler`)**
    - **作用**：处理服务实例向服务注册中心发起的注册请求。
    - **功能**：接收并处理`register_request`消息，将服务信息注册到注册中心。


* **✅ 注册响应消息处理器 (`register_response_message_handler`)**
    - **作用**：处理服务注册请求的响应。
    - **功能**：接收并处理`register_response`消息，确认服务注册结果。


* **🚪 关闭请求消息处理器 (`close_request_message_handler`)**
    - **作用**：处理客户端发起的关闭连接请求。
    - **功能**：接收并处理`close_request`消息，执行连接关闭逻辑。

## auth handlers (认证处理器)

* **🔄 重载服务器单元数据库消息处理器 (`reload_server_cell_db_message_handler`)**
    - **作用**：处理重新加载服务器单元数据库配置的请求。
    - **功能**：接收并处理 `reload_server_cell_db_request` 消息，触发 `server_cell_repository`
      单例从数据库中重新加载所有服务器单元的配置信息。


* **🔄 重载应用数据库消息处理器 (`reload_app_db_message_handler`)**
    - **作用**：处理重新加载应用数据库配置的请求。
    - **功能**：接收并处理`reload_app_db_message`消息，根据`app_id`触发`app_secret`单例从数据库中重新加载指定应用的密钥信息。


* **🔄 重载短信服务商数据库消息处理器 (`reload_sms_providers_db_message_handler`)**
    - **作用**：处理重新加载短信服务商数据库配置的请求。
    - **功能**：接收并处理 `reload_sms_providers_db_request` 消息，根据 `provider_id` 触发 `app_sms_providers`
      单例从数据库中重新加载指定服务商的配置信息。


* **🔄 重载邮箱服务商数据库消息处理器 (`reload_email_providers_db_message_handler`)**
    - **作用**：处理重新加载邮箱服务商数据库配置的请求。
    - **功能**：接收并处理 `reload_email_providers_db_request` 消息，根据 `provider_id` 触发 `app_email_providers`
      单例从数据库中重新加载指定服务商的配置信息。


* **🔄 重载SDK服务商数据库消息处理器 (`reload_sdk_providers_db_message_handler`)**
    - **作用**：处理重新加载SDK服务商数据库配置的请求。
    - **功能**：接收并处理 `reload_sdk_providers_db_request` 消息，根据 `sdk_id` 触发 `app_sdk_providers`
      单例从数据库中重新加载指定服务商的配置信息。

* **👋 游客登录HTTP请求处理器 (`guest_login_http_message_handler`)**
    - **作用**：处理客户端发起的游客登录`HTTP`请求。
    - **功能**：接收并处理`HTTP`请求，根据设备ID查找或创建新账户，并返回访问令牌。


* **📱 发送短信HTTP请求处理器 (`send_sms_http_message_handler`)**
    - **作用**：处理客户端发起的发送短信`HTTP`请求。
    - **功能**：接收并处理`HTTP`请求，根据手机号发送验证码。


* **📱 手机登录HTTP请求处理器 (`phone_login_http_message_handler`)**
    - **作用**：处理客户端发起的手机号登录`HTTP`请求。
    - **功能**：接收并处理`HTTP`请求，根据手机号和验证码进行登录。


* **📱 手机绑定HTTP请求处理器 (`phone_bind_http_message_handler`)**
    - **作用**：处理客户端发起的手机号绑定`HTTP`请求。
    - **功能**：接收并处理`HTTP`请求，根据手机号和验证码进行绑定。


* **✉️ 发送邮箱验证码HTTP请求处理器 (`send_email_http_message_handler`)**
    - **作用**：处理客户端发起的发送邮箱验证码`HTTP`请求。
    - **功能**：接收并处理`HTTP`请求，根据邮箱地址发送验证码。


* **✉️ 邮箱登录HTTP请求处理器 (`email_login_http_message_handler`)**
    - **作用**：处理客户端发起的邮箱登录`HTTP`请求。
    - **功能**：接收并处理`HTTP`请求，根据邮箱和验证码进行登录。


* **✉️ 邮箱绑定HTTP请求处理器 (`email_bind_http_message_handler`)**
    - **作用**：处理客户端发起的邮箱绑定`HTTP`请求。
    - **功能**：接收并处理`HTTP`请求，根据邮箱和验证码进行绑定。


* **🔑 密码登录HTTP请求处理器 (`password_login_http_message_handler`)**
    - **作用**：处理客户端发起的密码登录`HTTP`请求。
    - **功能**：接收并处理`HTTP`请求，根据用户名和密码进行登录。


* **🔑 密码绑定HTTP请求处理器 (`password_bind_http_message_handler`)**
    - **作用**：处理客户端发起的密码绑定`HTTP`请求。
    - **功能**：接收并处理`HTTP`请求，根据用户名和密码进行绑定。


* **🚀 SDK登录HTTP请求处理器 (`sdk_login_http_message_handler`)**
    - **作用**：处理客户端发起的SDK登录`HTTP`请求。
    - **功能**：接收并处理`HTTP`请求，根据SDK信息进行登录。


* **🚀 SDK绑定HTTP请求处理器 (`sdk_bind_http_message_handler`)**
    - **作用**：处理客户端发起的SDK绑定`HTTP`请求。
    - **功能**：接收并处理`HTTP`请求，根据SDK信息进行绑定。


* **🖥️ 登录服务器列表HTTP请求处理器 (`login_servers_message_handler`)**
    - **作用**：处理客户端获取游戏服务器列表的`HTTP`请求。
    - **功能**：接收并处理`HTTP`请求，调用`login_servers`服务，并返回包含服务器列表的`login_servers_response`。

## payment handlers (支付处理器)

* **🔄 重载SDK支付服务商数据库消息处理器 (`reload_sdk_payment_providers_db_message_handler`)**
    - **作用**：处理重新加载SDK支付服务商数据库配置的请求。
    - **功能**：接收并处理 `reload_sdk_payment_providers_db_request` 消息，根据 `provider_id` 触发
      `app_sdk_payment_providers` 单例从数据库中重新加载指定服务商的配置信息。


* **🔔 支付通知HTTP请求处理器 (`notify_http_message_handler`)**
    - **作用**：处理来自第三方平台的支付通知`HTTP`请求。
    - **功能**：接收并处理`HTTP`通知，验证通知的合法性，并更新对应的支付订单状态。


* **🔔 退款HTTP请求处理器 (`refund_http_message_handler`)**
    - **作用**：处理来自第三方平台的退款`HTTP`请求。
    - **功能**：接收并处理`HTTP`通知，验证通知的合法性，并更新对应的支付订单状态。

## player handlers (玩家处理器)

* **➡️ 登录请求消息处理器 (`service_login_request_message_handler`)**
    - **作用**：处理客户端发起的登录请求。`
    - **功能**：接收并处理`login_request`消息，验证玩家身份并返回登录结果。


* **🔄 重载配置数据库消息处理器 (`reload_config_db_message_handler`)**
    - **作用**：处理重新加载配置数据库的请求。
    - **功能**：接收并处理 `reload_config_db` 消息，触发 `config_manager` 单例从数据库中重新加载指定 `db_name` 和 `id`
      的配置信息。


* **🔄 重载游戏配置消息处理器 (`reload_game_config_message_handler`)**
    - **作用**：处理重新加载游戏配置的请求。
    - **功能**：接收并处理 `reload_game_config_request` 消息，触发 `game_config::load_tables()` 重新加载所有游戏配置表。

## http handlers（HTTP 处理器）

* **🌐 HTTP健康检查请求处理器 (`health_check_request_http_message_handler`)**
    - **作用**：处理来自客户端的`HTTP`健康检查请求。
    - **功能**：接收并处理`HTTP` `GET`请求，返回服务当前的健康状态。


* **📨 HTTP健康检查响应处理器 (`health_check_response_http_message_handler`)**
    - **作用**：处理`HTTP`健康检查请求的响应。
    - **功能**：接收并处理`HTTP`响应，通常用于内部服务间健康状态的同步或监控。

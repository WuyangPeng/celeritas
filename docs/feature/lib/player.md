# player（玩家）

玩家模块包含了与玩家核心业务相关的功能，例如登录流程管理。

## login (登陆)

* **⚙️ 玩家登录 (`service_login`)**
    - **作用**：作为处理玩家登录逻辑的核心类。
    - **功能**：封装了玩家登录的完整流程，包括验证、数据加载和会话管理。


* **🆕 创建账户 (`create_account`)**
    - **作用**：负责处理创建新玩家账户的逻辑。


* **🆕 创建用户 (`create_user`)**
    - **作用**：负责处理创建新用户的逻辑。

## core components (核心组件)

* **👨‍💼 玩家管理器 (`player_manager`)**
    - **作用**：一个单例管理器，负责创建、存储和提供对所有在线玩家（`player_state`）的全局访问。


* **🧍 玩家状态 (`player_state`)**
    - **作用**：表示一个玩家的完整状态，包括其所有组件。它管理着玩家的生命周期，如加载、在线、断线和登出。


* **🧩 玩家组件 (`player_component`)**
    - **作用**：所有玩家组件的抽象基类，定义了组件的生命周期钩子函数，如 `on_load_db`, `on_login`, `on_logout` 等。


* **🚫 玩家空组件 (`player_null_component`)**
    - **作用**：一个特殊的玩家组件，用于表示一个空的或无效的组件。
    - **特点**：通常用于占位或作为默认值，避免空指针异常。


* **🎮 玩家服务基类 (`player_service_base`)**
    - **作用**：定义了处理具体玩家业务逻辑的服务基类，专门用于处理 Protobuf 消息。
    - **接口**：
        - `response()`：纯虚函数，子类需实现具体的业务逻辑并生成响应。
        - `send_error_response()`：纯虚函数，子类需实现错误响应的发送逻辑。
    - **功能**：
        - 管理 `protobuf_handle_parameter` 和 `player_state`，为子类提供完整的请求上下文和玩家状态信息。
        - 提供了 `get_player_state()` 方法，方便子类访问玩家数据。

## user components (用户组件)

* **👤 玩家用户组件 (`player_user_component`)**
    - **作用**：一个具体的玩家组件，负责管理玩家的用户基础信息。

## role components (角色组件)

* **🎭 玩家角色组件 (`player_role_component`)**
    - **作用**：一个具体的玩家组件，负责管理玩家的角色信息。


* **📝 修改名称 (`change_name`)**
    - **作用**：处理玩家修改角色名称的业务逻辑。
    - **功能**：继承自 `player_service_base`，接收 `change_role_name_request` 请求，并更新 `player_role_component` 中的角色名称。

## online components (在线组件)

* **📶 玩家在线组件 (`player_online_component`)**
    - **作用**：一个具体的玩家组件，负责管理玩家的在线状态和相关数据。


* **💓 心跳 (`heartbeat`)**
    - **作用**：处理客户端与服务器之间的心跳消息，以维持连接的活跃状态。

## debug components (调试组件)

* **🐛 玩家调试组件 (`player_debug_component`)**
    - **作用**：一个具体的玩家组件，负责处理游戏中的调试和作弊命令。


* **🛠️ 调试基类 (`debug_base`)**
    - **作用**：所有调试命令处理类的基类，继承自 `player_service_base`。
    - **功能**：
        - 提供统一的接口 `do_response()` 供子类实现具体的调试逻辑。
        - 提供 `get_id()` 和 `get_parameter()` 等辅助方法获取调试参数。
        - 负责解析和分发具体的调试请求。


* **➕ 添加道具 (`add_item`)**
    - **作用**：处理添加道具的调试命令。
    - **功能**：继承自 `debug_base`，解析请求中的道具ID和数量，并调用 `player_item_component` 添加道具。

## time components (时间组件)

* **⏳ 玩家时间组件 (`player_time_component`)**
    - **作用**：一个具体的玩家组件，负责管理玩家的各种时间信息，如冷却时间、持续时间等。

* **🔑 玩家时间刷新键 (`player_time_refresh_key`)**
    - **作用**：作为玩家时间刷新事件的唯一标识符。
    - **特点**：通常与玩家组件类型（`player_component_type`）结合使用，用于注册、移除和触发特定的定时刷新逻辑，例如每日重置、冷却时间结束等。

## red dot components (红点组件)

* **🔴 玩家红点组件 (`player_red_dot_component`)**
    - **作用**：一个具体的玩家组件，负责管理玩家的红点系统。


* **🔴 红点节点 (`red_dot_node`)**
    - **作用**：表示游戏中的一个红点节点，用于构建红点树结构。
    - **特点**：包含红点类型、值、父节点和子节点。

## item components (道具组件)

* **🎒 玩家道具组件 (`player_item_component`)**
    - **作用**：一个具体的玩家组件，负责管理玩家的道具数据。


* **📦 道具容器 (`item_container`)**
    - **作用**：一个用于存储和管理多个 `item_info` 对象的容器。
    - **功能**：
        - **`add_item_info(template_id, count)`**：向容器中添加一个新的道具信息。
        - 提供迭代器 `begin()` 和 `end()` 以便遍历其中的所有道具。


* **ℹ️ 道具信息 (`item_info`)**
    - **作用**：表示一个具体的道具实例信息。
    - **内容**：包含 `template_id` (道具模板ID) 和 `count` (道具数量)。


* **✅ 道具选择 (`item_selected`)**
    - **作用**：处理玩家选择道具的业务逻辑。
    - **功能**：继承自 `player_service_base`，接收 `item_selected_request` 请求，并调用 `player_item_component`
      来处理道具的选择逻辑，例如更新道具状态、触发道具效果等。

## develop components (养成组件)

* **🛠️ 玩家养成组件 (`player_develop_component`)**
    - **作用**：一个具体的玩家组件，负责管理玩家的成长和发展相关数据。


* **🔑 养成数据键 (`develop_data_key`)**
    - **作用**：作为养成数据的唯一标识符。
    - **特点**：包含系统ID (`system_id`) 和实例ID (`instance_id`)，用于唯一标识一个养成数据项。


* **📈 养成升级 (`develop_level`)**
    - **作用**：处理玩家养成系统升级的业务逻辑。
    - **功能**：继承自 `player_service_base`，接收 `develop_level_request` 请求，并调用 `player_develop_component`
      来处理养成项的升级逻辑。


* **🔄 养成重置 (`develop_reset`)**
    - **作用**：处理玩家养成系统重置的业务逻辑。
    - **功能**：继承自 `player_service_base`，接收 `develop_reset_request` 请求，并调用 `player_develop_component`
      来处理养成项的重置逻辑。

## mail components (邮件组件)

* **📧 玩家邮件组件 (`player_mail_component`)**
    - **作用**：一个具体的玩家组件，负责管理玩家的邮件信息。

## task components (任务组件)

* **🎯 玩家任务组件 (`player_task_component`)**
    - **作用**：一个具体的玩家组件，负责管理玩家的任务进度和状态。

## activity components (活动组件)

* **🎉 玩家活动组件 (`player_activity_component`)**
    - **作用**：一个具体的玩家组件，负责管理玩家参与的活动信息。

## attribute components (属性组件)

* **💪 玩家属性组件 (`player_attribute_component`)**
    - **作用**：一个具体的玩家组件，负责管理玩家的动态属性。

## instance components (副本组件)

* **🗺️ 玩家副本组件 (`player_instance_component`)**
    - **作用**：一个具体的玩家组件，负责管理玩家的副本进度和状态。

## finish components (完成组件)

* **🏁 玩家完成组件 (`player_finish_component`)**
    - **作用**：一个具体的玩家组件，负责管理玩家的成就或完成度相关数据。

## server mail components (服务器邮件组件)

* **📧 服务器邮件管理器 (`server_mail_manager`)**
    - **作用**：一个单例管理器，负责管理系统邮件、活动邮件等服务器端邮件。
    - **功能**：
        - 加载所有服务器邮件数据
        - 根据ID、类型获取邮件
        - 添加、删除邮件
        - 清理过期邮件
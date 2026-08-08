# admin（后台管理）

后台管理模块（`admin`）负责处理系统管理员账号管理、身份鉴权、系统公告发布与广播管理以及玩家反馈查询等后台业务功能。该模块为 Web/HTTP 管理后台提供 API 接口支持与业务逻辑封装。

## 核心组件与公共定义 (Core Components & Common Definitions)

* **🎮 后台服务基类 (`admin_service_base`)**
    - **作用**：所有后台 HTTP 业务服务的抽象基类，继承自 `http_service_base`。
    - **功能**：
        - **会话鉴权**：提供 `resolve_operator_id(token)` 异步静态方法，通过 Redis 校验 Token 并解析出当前操作者的管理员 ID。
        - **账号获取**：提供 `get_admin_account(mysql_pool, target_id)` 异步静态方法，从 MySQL 数据库获取管理员账号信息。


* **📌 通用后台请求参数 (`admin_parameter`)**
    - **作用**：继承自 `http_handle_parameter`，作为所有后台 HTTP 请求参数类的通用基类，用于解析和携带基础请求信息。


* **🔑 后台公共常量 (`admin_constant`)**
    - **作用**：定义后台 API 通信中使用的 key 名称及公共常量。
    - **主要字段与常量**：
        - 账号相关：`account_id`, `username`, `password`, `role`, `nickname`, `avatar`, `status`, `app_id`, `token`
        - 公告相关：`id`, `app_id`, `title`, `tag`, `banner_url`, `content`, `priority`, `publish_time`, `status`, `server`
        - 查询与会话：`page`, `page_size`, `keyword`, `default_super_admin_username` ("admin"), `admin_session_ttl_seconds` (86400 秒 / 24小时)

---

## 账号管理 (Account Management)

管理员账号管理子模块涵盖管理员身份角色判定、账号生命周期控制（创建、登录、修改密码、修改状态、删除、列表查询）等功能。

### 关联数据类型 (Types)

* **👥 管理员角色类型 (`admin_role_type`)**
    - `super_admin` (0)：超级管理员
    - `operation_admin` (1)：运营管理员
    - `devops_admin` (2)：运维管理员
    - `developer_admin` (3)：程序管理员
    - `readonly_user` (4)：只读用户


* **🚦 管理员状态类型 (`admin_status_type`)**
    - `disabled` (0)：禁用状态
    - `enabled` (1)：启用状态

### 业务服务与响应 (Services & Response)

* **🔐 管理员登录 (`admin_login`)**
    - **作用**：处理管理员账号密码登录鉴权。
    - **特点**：
        - 校验账号是否存在及当前状态是否启用（`enabled`）。
        - 使用 SHA256 加密算法对输入的密码进行比对校验。
        - 验证通过后，在 Redis 中生成会话 Token 并设置过期时间（默认 24 小时）。
    - **相关类**：`admin_login_parameter`, `admin_login_response`


* **➕ 创建管理员账号 (`admin_create`)**
    - **作用**：创建新的管理员账号。
    - **特点**：检查用户名是否冲突，对初始密码进行 SHA256 哈希后写入数据库。
    - **相关类**：`admin_create_parameter`, `admin_create_response`


* **🔑 修改管理员密码 (`admin_update_password`)**
    - **作用**：修改指定管理员账号的密码。
    - **特点**：校验原密码（或操作者权限）后更新数据库中的密码哈希值。
    - **相关类**：`admin_update_password_parameter`, `admin_update_password_response`

* **⚙️ 修改管理员状态 (`admin_update_status`)**
    - **作用**：启用或禁用指定的管理员账号。
    - **相关类**：`admin_update_status_parameter`, `admin_update_status_response`


* **🗑️ 删除管理员账号 (`admin_delete`)**
    - **作用**：从系统中删除指定 ID 的管理员账号。
    - **相关类**：`admin_delete_parameter`, `admin_delete_response`


* **📋 管理员列表查询 (`admin_list`)**
    - **作用**：分页查询系统管理员列表。
    - **特点**：支持根据关键词（如用户名、昵称等）进行搜索过滤，返回符合条件的管理员列表与分页总数。
    - **相关类**：`admin_list_parameter`, `admin_list_item`, `admin_list_result`, `admin_list_response`

---

## 公告管理 (Announcement Management)

公告管理子模块提供游戏/系统公告的增删改查及实时在线广播功能。

### 关联数据类型 (Types)

* **📢 公告状态 (`announcement_status`)**
    - `draft` (0)：草稿箱
    - `published` (1)：已发布
    - `archived` (2)：已归档

### 业务服务与响应 (Services & Response)

* **📝 创建系统公告 (`announcement_admin_create`)**
    - **作用**：后台添加新的系统公告。
    - **特点**：支持配置关联 App ID、针对的具体服务器标识、公告标题、标签、Banner 图 URL、主体内容、优先级和预约发布时间。
    - **相关类**：`announcement_admin_create_parameter`, `announcement_admin_create_response`


* **✏️ 更新系统公告 (`announcement_admin_update`)**
    - **作用**：修改已有系统公告的内容、状态或发布参数。
    - **相关类**：`announcement_admin_update_parameter`, `announcement_admin_update_response`


* **🗑️ 删除系统公告 (`announcement_admin_delete`)**
    - **作用**：删除指定 ID 的系统公告。
    - **相关类**：`announcement_admin_delete_parameter`, `announcement_admin_delete_response`


* **📋 公告列表查询 (`announcement_admin_list`)**
    - **作用**：后台分页检索系统公告列表。
    - **特点**：支持按 App ID、服务器标识、公告状态、关键词等组合条件进行分页筛选。
    - **相关类**：`announcement_admin_list_parameter`, `announcement_admin_list_item`, `announcement_admin_list_result`, `announcement_admin_list_response`


* **📡 广播系统公告 (`announcement_admin_broadcast`)**
    - **作用**：将指定的系统公告向符合条件的服务节点或在线玩家进行实时推播。
    - **相关类**：`announcement_admin_broadcast_parameter`, `announcement_admin_broadcast_response`

---

## 反馈管理 (Feedback Management)

反馈管理子模块用于接收与查阅玩家在客户端提交的各类反馈数据。

### 关联数据类型 (Types)

* **💬 反馈类型 (`feedback_type`)**
    - `bug` (0)：问题缺陷报告
    - `suggestion` (1)：意见与建议
    - `report` (2)：举报与投诉

### 业务服务与响应 (Services & Response)

* **📋 玩家反馈列表查询 (`feedback_admin_list`)**
    - **作用**：后台分页检索玩家提交的反馈记录。
    - **特点**：支持筛选特定反馈类型（Bug / 建议 / 举报）、时间区间及关键词搜索，帮助运营团队快速响应玩家诉求。
    - **相关类**：`feedback_admin_list_parameter`, `feedback_admin_list_item`, `feedback_admin_list_result`, `feedback_admin_list_response`

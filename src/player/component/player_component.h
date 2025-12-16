#pragma once

#include "database/database_fwd.h"
#include "player/player_fwd.h"

#include <boost/asio/awaitable.hpp>

namespace celeritas
{
    class player_component : public std::enable_shared_from_this<player_component>
    {
    public:
        using class_type = player_component;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using database_pool_shared_ptr = std::shared_ptr<database_pool_base>;

        player_component(player_component_type player_component_type, player_state* player_state) noexcept;

        virtual ~player_component() noexcept = default;

        player_component(const player_component& rhs) = default;

        player_component& operator=(const player_component& rhs) = default;

        player_component(player_component&& rhs) noexcept = default;

        player_component& operator=(player_component&& rhs) noexcept = default;

        [[nodiscard]] player_component_type get_player_component_type() const noexcept;

        // 数据库数据加载
        [[nodiscard]] virtual void_awaitable_type on_load_db();

        // 数据库数据解析
        [[nodiscard]] virtual void_awaitable_type on_db_analysis();

        // 安全地访问其他组件的数据，解决组件间的依赖关系。
        [[nodiscard]] virtual void_awaitable_type on_dependencies_ready();

        // 执行所有的初始同步消息发送。
        [[nodiscard]] virtual void_awaitable_type send_initial_sync();

        // 所有数据稳定后，执行最终启动逻辑。
        [[nodiscard]] virtual void_awaitable_type on_login();

        // 玩家登出
        [[nodiscard]] virtual void_awaitable_type on_logout();

        // 保存数据库
        [[nodiscard]] virtual void_awaitable_type save_db();

        // 默认定时器回调
        [[nodiscard]] virtual void_awaitable_type time_callback(time_refresh_type time_refresh_type, int64_t parameter, int64_t time_id, bool is_login);

        [[nodiscard]] player_state* get_player_state();

        [[nodiscard]] database_pool_shared_ptr get_mysql_player_db_name();

        [[nodiscard]] database_pool_shared_ptr get_mongo_player_db_name();

        [[nodiscard]] virtual bool is_modify() const;

        // 测试所需接口
        void set_mock_database_pool(const database_pool_shared_ptr& database_pool);

    private:
        using optional_database_pool_shared_ptr = std::optional<database_pool_shared_ptr>;

        player_component_type player_component_;
        player_state* player_state_;
        optional_database_pool_shared_ptr mock_database_pool_;
    };
}
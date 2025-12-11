#pragma once

#include "player/player_fwd.h"

#include <boost/asio/awaitable.hpp>

namespace celeritas
{
    class player_component
    {
    public:
        using class_type = player_component;
        using void_awaitable_type = boost::asio::awaitable<void>;

        explicit player_component(player_component_type player_component_type, player_state* player_state) noexcept;

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

        [[nodiscard]] player_state* get_player_state();

    private:
        player_component_type player_component_;
        player_state* player_state_;
    };
}
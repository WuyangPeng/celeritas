#pragma once

#include "player_event_dispatcher.h"
#include "player/component/player_component.h"

namespace celeritas
{
    // 玩家事件组件
    class player_event_component : public player_component
    {
    public:
        using class_type = player_event_component;
        using player_event_shared_ptr = std::shared_ptr<player_event>;
        using player_event_listener_shared_ptr = std::shared_ptr<player_event_listener>;
        using listener_id_type = player_event_dispatcher::listener_id_type;

        explicit player_event_component(player_state* player_state) noexcept;

        ~player_event_component() noexcept override = default;

        // 注册事件监听器
        [[nodiscard]] listener_id_type register_listener(player_event_type event_type, const player_event_listener_shared_ptr& listener);

        // 注销事件监听器
        void unregister_listener(player_event_type event_type, listener_id_type listener_id);

        // 触发事件
        [[nodiscard]] void_awaitable_type trigger_event(const player_event_shared_ptr& event);

        // 创建并触发事件
        [[nodiscard]] void_awaitable_type trigger_event(player_event_type event_type);

        [[nodiscard]] void_awaitable_type on_logout() override;

    private:
        player_event_dispatcher dispatcher_;
    };
}

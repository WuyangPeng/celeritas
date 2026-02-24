#pragma once

#include "player_event.h"
#include "player_event_priority.h"

#include <boost/asio/awaitable.hpp>

#include <memory>

namespace celeritas
{
    // 事件监听器接口
    class player_event_listener
    {
    public:
        using class_type = player_event_listener;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using player_event_shared_ptr = std::shared_ptr<player_event>;

        virtual ~player_event_listener() noexcept = default;

        player_event_listener(const player_event_listener& rhs) = default;

        player_event_listener& operator=(const player_event_listener& rhs) = default;

        player_event_listener(player_event_listener&& rhs) noexcept = default;

        player_event_listener& operator=(player_event_listener&& rhs) noexcept = default;

        // 处理事件
        [[nodiscard]] virtual void_awaitable_type on_event(const player_event_shared_ptr& event) = 0;

        // 获取监听器优先级（数值越小优先级越高）
        [[nodiscard]] virtual player_event_priority get_priority() const noexcept;

    protected:
        player_event_listener() noexcept = default;
    };
}

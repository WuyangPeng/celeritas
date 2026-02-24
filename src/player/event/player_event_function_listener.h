#pragma once

#include "player_event_listener.h"

#include <functional>

namespace celeritas
{
    // 函数式监听器
    class player_event_function_listener : public player_event_listener
    {
    public:
        using class_type = player_event_function_listener;
        using event_handler_type = std::function<void_awaitable_type(const player_event_shared_ptr&)>;

        explicit player_event_function_listener(event_handler_type handler, player_event_priority priority = player_event_priority::normal);

        [[nodiscard]] void_awaitable_type on_event(const player_event_shared_ptr& event) override;

        [[nodiscard]] player_event_priority get_priority() const noexcept override;

    private:
        event_handler_type handler_;
        player_event_priority priority_;
    };
}

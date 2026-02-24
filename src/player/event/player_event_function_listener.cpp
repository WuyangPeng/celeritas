#include "player_event_function_listener.h"

celeritas::player_event_function_listener::player_event_function_listener(event_handler_type handler, const player_event_priority priority)
    : handler_{ std::move(handler) }, priority_{ priority }
{
}

celeritas::player_event_function_listener::void_awaitable_type celeritas::player_event_function_listener::on_event(const player_event_shared_ptr& event)
{
    return handler_(event);
}

celeritas::player_event_priority celeritas::player_event_function_listener::get_priority() const noexcept
{
    return priority_;
}

#include "player_event_component.h"

celeritas::player_event_component::player_event_component(player_state* player_state) noexcept
    : player_component{ get_player_component_type(), player_state }
{
}

int64_t celeritas::player_event_component::register_listener(const player_event_type event_type, const player_event_listener_shared_ptr& listener)
{
    return dispatcher_.register_listener(event_type, listener);
}

void celeritas::player_event_component::unregister_listener(const player_event_type event_type, const int64_t listener_id)
{
    dispatcher_.unregister_listener(event_type, listener_id);
}

celeritas::player_event_component::void_awaitable_type celeritas::player_event_component::trigger_event(const player_event_shared_ptr& event)
{
    co_await dispatcher_.dispatch(event);
}

celeritas::player_event_component::void_awaitable_type celeritas::player_event_component::trigger_event(const player_event_type event_type, const bool is_login)
{
    const auto event = std::make_shared<player_event>(event_type, is_login);
    co_await trigger_event(event);
}

celeritas::player_event_component::void_awaitable_type celeritas::player_event_component::on_logout()
{
    dispatcher_.clear();
    co_return;
}

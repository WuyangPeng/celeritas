#include "mock_player_component.h"

celeritas::mock_player_component::mock_player_component(player_state* player_state) noexcept
    : base_type{ get_player_component_type(), player_state }, time_callback_is_login_{}, time_callback_is_not_login_{}
{
}

celeritas::player_component::void_awaitable_type celeritas::mock_player_component::time_callback(const player_time_refresh_key& player_time_refresh_key, bool is_login)
{
    if (is_login)
    {
        ++time_callback_is_login_;
    }
    else
    {
        ++time_callback_is_not_login_;
    }

    co_return;
}

int celeritas::mock_player_component::get_time_callback_is_login() const
{
    return time_callback_is_login_;
}

int celeritas::mock_player_component::get_time_callback_is_not_login() const
{
    return time_callback_is_not_login_;
}
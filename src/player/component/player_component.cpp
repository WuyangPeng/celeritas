#include "player_component.h"

celeritas::player_component::player_component(player_component_type player_component_type, player_state* player_state) noexcept
    : player_component_{ player_component_type }, player_state_{ player_state }
{
}

celeritas::player_component_type celeritas::player_component::get_player_component_type() const noexcept
{
    return player_component_;
}

celeritas::player_component::void_awaitable_type celeritas::player_component::on_load_db()
{
    co_return;
}

celeritas::player_component::void_awaitable_type celeritas::player_component::on_db_analysis()
{
    co_return;
}

celeritas::player_component::void_awaitable_type celeritas::player_component::on_dependencies_ready()
{
    co_return;
}

celeritas::player_component::void_awaitable_type celeritas::player_component::send_initial_sync()
{
    co_return;
}

celeritas::player_component::void_awaitable_type celeritas::player_component::on_login()
{
    co_return;
}

celeritas::player_component::void_awaitable_type celeritas::player_component::on_logout()
{
    co_return;
}

celeritas::player_component::void_awaitable_type celeritas::player_component::save_db()
{
    co_return;
}

celeritas::player_state* celeritas::player_component::get_player_state()
{
    return player_state_;
}
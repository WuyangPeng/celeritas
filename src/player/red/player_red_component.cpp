#include "player_red_component.h"

celeritas::player_red_component::player_red_component(player_state* player_state) noexcept
    : base_type{ get_player_component_type(), player_state }
{
}

celeritas::player_component::void_awaitable_type celeritas::player_red_component::on_load_db()
{
    return player_component::on_load_db();
}
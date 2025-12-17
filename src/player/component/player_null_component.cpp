#include "player_null_component.h"

celeritas::player_null_component::player_null_component(player_state* player_state) noexcept
    : base_type{ get_player_component_type(), player_state }
{
}
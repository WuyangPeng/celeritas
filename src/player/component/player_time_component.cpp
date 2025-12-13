#include "player_time_component.h"

celeritas::player_time_component::player_time_component(player_state* player_state) noexcept
    : base_type{ get_player_component_type(), player_state }
{
}
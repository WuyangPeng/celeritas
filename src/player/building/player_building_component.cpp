#include "player_building_component.h"

celeritas::player_building_component::player_building_component(player_state* player_state) noexcept
    : base_type{ get_player_component_type(), player_state }
{
}
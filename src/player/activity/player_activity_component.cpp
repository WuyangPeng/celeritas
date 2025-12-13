#include "player_activity_component.h"

celeritas::player_activity_component::player_activity_component(player_state* player_state) noexcept
    : base_type{ get_player_component_type(), player_state }
{
}
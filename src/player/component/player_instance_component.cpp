#include "player_instance_component.h"

celeritas::player_instance_component::player_instance_component(player_state* player_state) noexcept
    : base_type{ get_player_component_type(), player_state }
{
}
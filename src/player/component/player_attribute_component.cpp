#include "player_attribute_component.h"

celeritas::player_attribute_component::player_attribute_component(player_state* player_state) noexcept
    : base_type{ get_player_component_type(), player_state }
{
}
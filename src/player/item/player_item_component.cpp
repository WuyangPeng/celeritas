#include "player_item_component.h"

celeritas::player_item_component::player_item_component(player_state* player_state) noexcept
    : base_type{ get_player_component_type(), player_state }
{
}
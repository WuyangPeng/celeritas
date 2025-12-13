#include "player_develop_component.h"

celeritas::player_develop_component::player_develop_component(player_state* player_state) noexcept
    : base_type{ get_player_component_type(), player_state }
{
}
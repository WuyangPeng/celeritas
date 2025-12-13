#include "player_role_component.h"

celeritas::player_role_component::player_role_component(player_state* player_state) noexcept
    : base_type{ get_player_component_type(), player_state }
{
}

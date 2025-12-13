#include "player_task_component.h"

celeritas::player_task_component::player_task_component(player_state* player_state) noexcept
    : base_type{ get_player_component_type(), player_state }
{
}
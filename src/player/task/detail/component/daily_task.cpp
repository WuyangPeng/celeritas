#include "daily_task.h"

celeritas::daily_task::daily_task(player_state* player_state) noexcept
    : base_type{ player_state }
{
}

celeritas::task::task_component_type celeritas::daily_task::get_task_component_type() const
{
    return task_component_type::daily;
}


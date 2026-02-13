#include "daily_task.h"

celeritas::daily_task::daily_task(player_state* player_state) noexcept
    : base_type{ player_state }
{
}

celeritas::task::task_component_type celeritas::daily_task::get_task_component_type() const
{
    return task_component_type::daily;
}

celeritas::task::void_awaitable_type celeritas::daily_task::on_db_analysis(const const_app_config_shared_ptr& app_config)
{
    co_return;
}


#include "title_task.h"

celeritas::title_task::title_task(player_state* player_state) noexcept
    : base_type{ player_state }
{
}

celeritas::task::task_component_type celeritas::title_task::get_task_component_type() const
{
    return task_component_type::title;
}

celeritas::task::void_awaitable_type celeritas::title_task::on_db_analysis(const const_app_config_shared_ptr& app_config)
{
    co_return;
}


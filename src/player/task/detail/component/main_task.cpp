#include "main_task.h"

celeritas::main_task::main_task(player_state* player_state) noexcept
    : base_type{ player_state }
{
}

celeritas::task::task_component_type celeritas::main_task::get_task_component_type() const
{
    return task_component_type::main;
}

celeritas::task::void_awaitable_type celeritas::main_task::on_db_analysis(const const_app_config_shared_ptr& app_config)
{
    co_return;
}


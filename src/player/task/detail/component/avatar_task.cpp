#include "avatar_task.h"
#include "common/core/snowflake_generator.h"
#include "config/aggregate/app_config.h"
#include "config/game/game_config.h"
#include "config/game/game_tables.h"
#include "config/game/pretreatment_config.h"
#include "player/component/player_state.h"
#include "player/task/player_task_component.h"

celeritas::avatar_task::avatar_task(player_state* player_state) noexcept
    : base_type{ player_state }
{
}

celeritas::task::task_component_type celeritas::avatar_task::get_task_component_type() const
{
    return task_component_type::avatar;
}

celeritas::task::void_awaitable_type celeritas::avatar_task::on_db_analysis(const const_app_config_shared_ptr& app_config)
{
    const auto task_config = game_config::get_instance().get_game_tables()->get_pretreatment_config()->get_task_config();
    const auto optional_tasks = task_config->get_tasks_by_type(task_component_type::avatar);
    const auto server_config = app_config->get_server_config();
    const auto task_component = player_state_->get_component<player_task_component>();

    if (const auto& tasks = *optional_tasks)
    {
        for (const auto& task : *tasks)
        {
            const auto item_id = snowflake_generator::get_instance().generate(server_config->get_datacenter_id(), server_config->get_worker_id());

            const auto progress = std::make_shared<task_progress>(item_id, task->id, task_component->get_default_progress(task->taskEventType, task->targetId));

            add_task_progress(progress);
        }
    }

    co_return;
}


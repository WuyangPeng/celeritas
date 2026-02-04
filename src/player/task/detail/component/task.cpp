#include "task.h"
#include "common/core/celeritas_error.h"
#include "config/game/game_config.h"
#include "config/game/game_tables.h"
#include "player/task/detail/base/task_context.h"
#include "../../../../database/document/task_progress.h"
#include "player/task/detail/condition/task_condition.h"

celeritas::task::task(player_state* player_state) noexcept
    : player_state_{ player_state }, task_progress_{}, event_{}, finish_task_ids_{}
{
}

celeritas::config::task_event_type celeritas::task::get_task_event_type(int cfg_id)
{
    const auto config = game_config::get_instance().get_game_tables()->get_tables()->task_config_container.get(cfg_id);
    if (!config)
    {
        throw celeritas_error{ "task event type is not exist,id = {}", cfg_id };
    }

    return (*config)->taskEventType;
}

void celeritas::task::add_task_progress(const task_progress_shared_ptr& task_progress)
{
    task_progress_.emplace(task_progress->get_id(), task_progress);

    const auto event_type = get_task_event_type(task_progress->get_cfg_id());
    const auto iter = event_.find(event_type);
    if (iter == event_.cend())
    {
        event_.emplace(event_type, std::set{ task_progress->get_id() });

        return;
    }

    iter->second.emplace(task_progress->get_id());
}

void celeritas::task::update_task_progress(const task_context& task_context)
{
    const auto event_type = task_context.get_type();
    if (const auto iter = event_.find(event_type);
        iter != event_.cend())
    {
        const auto& task_config_container = game_config::get_instance().get_game_tables()->get_tables()->task_config_container;
        for (const auto& element : iter->second)
        {
            if (auto task_progress_iter = task_progress_.find(element);
                task_progress_iter != task_progress_.cend())
            {
                if (const auto optional_config = task_config_container.get(task_progress_iter->second->get_cfg_id()))
                {
                    const auto& config = *optional_config;
                    if (config->progress <= task_progress_iter->second->get_progress())
                    {
                        finish_task(element);
                        continue;
                    }

                    if (const auto task_condition = task_condition::create(event_type, config->targetId);
                        task_condition->apply(*task_progress_iter->second, task_context))
                    {
                        if (config->progress <= task_progress_iter->second->get_progress())
                        {
                            finish_task(element);
                        }
                    }
                }
            }
        }
    }
}

void celeritas::task::finish_task(int64_t id)
{
    finish_task_ids_.emplace(id);

    if (const auto iter = task_progress_.find(id);
        iter != task_progress_.cend())
    {
        const auto event_type = get_task_event_type(iter->second->get_cfg_id());
        if (const auto event_iter = event_.find(event_type);
            event_iter != event_.cend())
        {
            event_iter->second.erase(id);
            if (event_iter->second.empty())
            {
                event_.erase(event_iter);
            }
        }
        task_progress_.erase(iter);
    }
}

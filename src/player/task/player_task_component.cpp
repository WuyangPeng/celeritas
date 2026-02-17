#include "player_task_component.h"
#include "detail/component/avatar_task.h"
#include "detail/component/daily_task.h"
#include "detail/component/frame_task.h"
#include "detail/component/main_task.h"
#include "detail/default_progress/kill_monster_progress_calculator.h"
#include "detail/default_progress/upgrade_level_progress_calculator.h"
#include "detail/default_progress/hold_item_progress_calculator.h"

celeritas::player_task_component::player_task_component(player_state* player_state) noexcept
    : base_type{ get_player_component_type(), player_state },
      tasks_{ std::make_shared<main_task>(player_state),
              std::make_shared<daily_task>(player_state),
              std::make_shared<avatar_task>(player_state),
              std::make_shared<frame_task>(player_state) },
      calculators_{ { config::task_event_type::kill_monster, std::make_shared<kill_monster_progress_calculator>(player_state) },
                    { config::task_event_type::upgrade_level, std::make_shared<upgrade_level_progress_calculator>(player_state) },
                    { config::task_event_type::hold_item, std::make_shared<hold_item_progress_calculator>(player_state) } }
{
}

celeritas::player_component::void_awaitable_type celeritas::player_task_component::on_db_analysis(const const_app_config_shared_ptr& app_config)
{
    for (const auto& element : tasks_)
    {
        co_await element->on_db_analysis(app_config);
    }
}

int celeritas::player_task_component::get_default_progress(const config::task_event_type task_event_type, const int target_id) const
{
    if (const auto it = calculators_.find(task_event_type);
        it != calculators_.cend())
    {
        return it->second->calculate(task_event_type, target_id);
    }
    return 0;
}

int celeritas::player_task_component::get_progress(const config::task_component_type task_component_type, const int cfg_id) const
{
    return tasks_.at(enum_cast_underlying(task_component_type))->get_progress_by_cfg_id(cfg_id);
}

void celeritas::player_task_component::update_task_progress(const task_context& task_context, bool is_login)
{
    for (const auto& element : tasks_)
    {
        element->update_task_progress(task_context);
    }
}

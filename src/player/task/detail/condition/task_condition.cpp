#include "hold_item_condition.h"
#include "kill_monster_condition.h"
#include "task_condition.h"
#include "upgrade_level_condition.h"
#include "common/core/celeritas_error.h"
#include "database/document/task_progress.h"
#include "player/task/detail/base/task_change_type.h"
#include "player/task/detail/base/task_context.h"

celeritas::task_condition::task_condition_unique_ptr celeritas::task_condition::create(const config::task_event_type type, int target_id)
{
    switch (type)
    {
        case config::task_event_type::kill_monster:
            return std::make_unique<kill_monster_condition>(target_id);
        case config::task_event_type::upgrade_level:
            return std::make_unique<upgrade_level_condition>(target_id);
        case config::task_event_type::hold_item:
            return std::make_unique<hold_item_condition>(target_id);
        default:
            throw celeritas_error{ "unsupported task_event_type" };
    }
}

bool celeritas::task_condition::set_progress(task_progress& progress, const task_context& context) const
{
    if (context.get_type() != get_event_type())
    {
        return false;
    }

    const auto change = context.get_change_type();
    if (change == task_change_type::add)
    {
        progress.add_progress(context.get_value());
        return true;
    }
    if (change == task_change_type::set)
    {
        progress.set_progress(context.get_value());
        return true;
    }
    return false;
}
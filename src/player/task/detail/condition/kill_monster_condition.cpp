#include "kill_monster_condition.h"
#include "player/task/detail/base/task_context.h"

celeritas::kill_monster_condition::kill_monster_condition(int target_monster_id)
    : base_type{}, target_monster_id_{ target_monster_id }
{
}

celeritas::config::task_event_type celeritas::kill_monster_condition::get_event_type() const
{
    return config::task_event_type::kill_monster;
}

bool celeritas::kill_monster_condition::apply(task_progress& progress, const task_context& context) const
{
    if (context.get_target_id() != target_monster_id_)
    {
        return false;
    }

    return set_progress(progress, context);
}

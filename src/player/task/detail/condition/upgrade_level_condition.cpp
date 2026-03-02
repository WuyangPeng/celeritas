#include "upgrade_level_condition.h"
#include "player/task/detail/base/task_context.h"

celeritas::upgrade_level_condition::upgrade_level_condition(const int develop_id)
    : base_type{}, develop_id_{ develop_id }
{
}

celeritas::config::task_event_type celeritas::upgrade_level_condition::get_event_type() const
{
    return config::task_event_type::upgrade_level;
}

bool celeritas::upgrade_level_condition::apply(task_progress& progress, const task_context& context) const
{
    if (context.get_target_id() != develop_id_)
    {
        return false;
    }

    return set_progress(progress, context);
}

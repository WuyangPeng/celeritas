#include "hold_item_condition.h"
#include "player/task/detail/base/task_context.h"

celeritas::hold_item_condition::hold_item_condition(int item_id)
    : base_type{}, item_id_{ item_id }
{
}

celeritas::config::task_event_type celeritas::hold_item_condition::get_event_type() const
{
    return config::task_event_type::hold_item;
}

bool celeritas::hold_item_condition::apply(task_progress& progress, const task_context& context) const
{
    if (context.get_target_id() != item_id_)
    {
        return false;
    }

    return set_progress(progress, context);
}

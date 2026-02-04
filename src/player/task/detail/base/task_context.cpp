#include "task_context.h"

celeritas::task_context::task_context(const config::task_event_type type, const task_change_type change_type, const int target_id, const int value)
    : type_{ type }, change_type_{ change_type }, target_id_{ target_id }, value_{ value }
{
}

celeritas::task_context::task_context(const config::task_event_type type, const task_change_type change_type, const int value)
    : class_type{ type, change_type, 0, value }
{
}

celeritas::config::task_event_type celeritas::task_context::get_type() const
{
    return type_;
}

celeritas::task_change_type celeritas::task_context::get_change_type() const
{
    return change_type_;
}

int celeritas::task_context::get_target_id() const
{
    return target_id_;
}

int celeritas::task_context::get_value() const
{
    return value_;
}
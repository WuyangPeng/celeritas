#pragma once

#include "player/task/detail/player_task_internal_fwd.h"
#include "config/luban/generated/schema.h"

namespace celeritas
{
    class task_context
    {
    public:
        using class_type = task_context;

        task_context(config::task_event_type type, task_change_type change_type, int target_id, int64_t value);

        task_context(config::task_event_type type, task_change_type change_type, int64_t value);

        [[nodiscard]] config::task_event_type get_type() const;

        [[nodiscard]] task_change_type get_change_type() const;

        [[nodiscard]] int get_target_id() const;

        [[nodiscard]] int64_t get_value() const;

    private:
        config::task_event_type type_;
        task_change_type change_type_;
        int target_id_;
        int64_t value_;
    };
}
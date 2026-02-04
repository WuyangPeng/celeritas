#pragma once

#include "player/task/detail/player_task_internal_fwd.h"
#include "config/luban/generated/schema.h"

namespace celeritas
{
    class task_condition
    {
    public:
        using class_type = task_condition;
        using task_condition_unique_ptr = std::unique_ptr<class_type>;

        virtual ~task_condition() noexcept = default;

        [[nodiscard]] virtual config::task_event_type get_event_type() const = 0;

        [[nodiscard]] virtual bool apply(task_progress& progress, const task_context& context) const = 0;

        [[nodiscard]] static task_condition_unique_ptr create(config::task_event_type type, int target_id);

        [[nodiscard]] bool set_progress(task_progress& progress, const task_context& context) const;
    };
}

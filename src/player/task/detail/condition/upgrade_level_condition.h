#pragma once

#include "task_condition.h"

namespace celeritas
{
    class upgrade_level_condition final : public task_condition
    {
    public:
        using class_type = upgrade_level_condition;
        using base_type = task_condition;

        [[nodiscard]] config::task_event_type get_event_type() const override;

        [[nodiscard]] bool apply(task_progress& progress, const task_context& context) const override;
    };
}

#pragma once

#include "task_condition.h"

namespace celeritas
{
    class kill_monster_condition final : public task_condition
    {
    public:
        using class_type = kill_monster_condition;
        using base_type = task_condition;

        explicit kill_monster_condition(int target_monster_id);

        [[nodiscard]] config::task_event_type get_event_type() const override;

        [[nodiscard]] bool apply(task_progress& progress, const task_context& context) const override;

    private:
        int target_monster_id_;
    };
}

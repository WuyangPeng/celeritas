#pragma once

#include "task_condition.h"

namespace celeritas
{
    class hold_item_condition final : public task_condition
    {
    public:
        using class_type = hold_item_condition;
        using base_type = task_condition;

        explicit hold_item_condition(int item_id);

        [[nodiscard]] config::task_event_type get_event_type() const override;

        [[nodiscard]] bool apply(task_progress& progress, const task_context& context) const override;

    private:
        int item_id_;
    };
}

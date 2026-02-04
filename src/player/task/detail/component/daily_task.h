#pragma once

#include "task.h"

namespace celeritas
{
    class daily_task final : public task
    {
    public:
        using class_type = daily_task;
        using base_type = task;

        explicit daily_task(player_state* player_state) noexcept;

        [[nodiscard]] task_type get_task_component_type() const override;
    };
}

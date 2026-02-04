#pragma once

#include "task.h"

namespace celeritas
{
    class main_task final : public task
    {
    public:
        using class_type = main_task;
        using base_type = task;

        explicit main_task(player_state* player_state) noexcept;

        [[nodiscard]] task_component_type get_task_component_type() const override;    };
}

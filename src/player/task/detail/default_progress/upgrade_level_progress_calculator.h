#pragma once

#include "default_progress_calculator.h"

namespace celeritas
{
    class upgrade_level_progress_calculator final : public default_progress_calculator
    {
    public:
        using base_type = default_progress_calculator;

        explicit upgrade_level_progress_calculator(player_state* player_state) noexcept;

        [[nodiscard]] int calculate(config::task_event_type task_event_type, int target_id) const override;
    };
}

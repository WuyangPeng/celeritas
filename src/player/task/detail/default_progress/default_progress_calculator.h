#pragma once

#include "config/luban/generated/schema.h"
#include "player/player_fwd.h"

namespace celeritas
{
    class default_progress_calculator
    {
    public:
        explicit default_progress_calculator(player_state* player_state) noexcept;

        virtual ~default_progress_calculator() = default;

        [[nodiscard]] virtual int calculate(config::task_event_type task_event_type, int target_id) const = 0;

    protected:
        player_state* player_state_;
    };
}

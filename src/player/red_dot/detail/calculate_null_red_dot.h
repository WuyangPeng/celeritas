#pragma once

#include "calculate_red_dot.h"

namespace celeritas
{
    class calculate_null_red_dot final : public calculate_red_dot
    {
    public:
        using class_type = calculate_null_red_dot;
        using base_type = calculate_red_dot;

        calculate_null_red_dot(player_state* player_state);

        [[nodiscard]] int get_red_dot_value() const override;
    };
}
#pragma once

#include "config/config_fwd.h"
#include "config/luban/generated/schema.h"
#include "player/player_fwd.h"

#include <memory>

namespace celeritas
{
    class calculate_red_dot
    {
    public:
        using class_type = calculate_red_dot;
        using red_dot_type = config::red_dot_type;
        using calculate_red_dot_shared_ptr = std::shared_ptr<class_type>;

        explicit calculate_red_dot(player_state* player_state) noexcept;

        virtual ~calculate_red_dot() noexcept = default;

        calculate_red_dot(const calculate_red_dot& rhs) = default;

        calculate_red_dot& operator=(const calculate_red_dot& rhs) = default;

        calculate_red_dot(calculate_red_dot&& rhs) = default;

        calculate_red_dot& operator=(calculate_red_dot&& rhs) = default;

        [[nodiscard]] virtual int get_red_dot_value() const = 0;

        [[nodiscard]] static calculate_red_dot_shared_ptr create(red_dot_type red_type, player_state* player_state);

    private:
        player_state* player_state_;
    };
}
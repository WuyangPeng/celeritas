#pragma once

#include "player_component.h"
#include "player_component_type.h"

namespace celeritas
{
    class player_task_component final : public player_component
    {
    public:
        using class_type = player_task_component;
        using base_type = player_component;

        explicit player_task_component(player_state* player_state) noexcept;

        [[nodiscard]] static constexpr player_component_type get_player_component_type()
        {
            return player_component_type::task;
        }
    };
}
#pragma once

#include "player/component/player_component.h"
#include "player/component/player_component_type.h"

namespace celeritas
{
    class player_develop_component final : public player_component
    {
    public:
        using class_type = player_develop_component;
        using base_type = player_component;

        explicit player_develop_component(player_state* player_state) noexcept;

        [[nodiscard]] static constexpr player_component_type get_player_component_type()
        {
            return player_component_type::develop;
        }
    };
}


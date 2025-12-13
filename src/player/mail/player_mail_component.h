#pragma once

#include "player/component/player_component.h"
#include "player/component/player_component_type.h"

namespace celeritas
{
    class player_mail_component final : public player_component
    {
    public:
        using class_type = player_mail_component;
        using base_type = player_component;

        explicit player_mail_component(player_state* player_state) noexcept;

        [[nodiscard]] static constexpr player_component_type get_player_component_type()
        {
            return player_component_type::mail;
        }
    };
}
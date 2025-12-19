#pragma once

#include "player/component/player_component.h"
#include "player/component/player_component_type.h"

namespace celeritas
{
    class mock_player_component final : public player_component
    {
    public:
        using class_type = mock_player_component;
        using base_type = player_component;

        explicit mock_player_component(player_state* player_state) noexcept;

        [[nodiscard]] static constexpr player_component_type get_player_component_type()
        {
            return player_component_type::mock;
        }

        [[nodiscard]] void_awaitable_type time_callback(const player_time_refresh_key& player_time_refresh_key, bool is_login) override;

        [[nodiscard]] int get_time_callback_is_login() const;

        [[nodiscard]] int get_time_callback_is_not_login() const;

    private:
        int time_callback_is_login_;
        int time_callback_is_not_login_;
    };
}
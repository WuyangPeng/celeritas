#pragma once

#include "player/component/player_component.h"
#include "player/component/player_component_type.h"

namespace celeritas
{
    class player_debug_component final : public player_component
    {
    public:
        using class_type = player_debug_component;
        using base_type = player_component;

        explicit player_debug_component(player_state* player_state) noexcept;

        [[nodiscard]] static constexpr player_component_type get_player_component_type()
        {
            return player_component_type::debug;
        }

        [[nodiscard]] void_awaitable_type on_db_analysis(const const_app_config_shared_ptr& app_config) override;

        [[nodiscard]] void_awaitable_type send_initial_sync() override;

        [[nodiscard]] bool is_debug() const;

    private:
        bool is_debug_;
    };
}
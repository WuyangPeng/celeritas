#pragma once

#include "database/generated/mongo/auth/user_server_roles.h"
#include "player/component/player_component.h"
#include "player/component/player_component_type.h"

namespace celeritas
{
    class player_time_component final : public player_component
    {
    public:
        using class_type = player_user_component;
        using base_type = player_component;

        explicit player_time_component(player_state* player_state) noexcept;

        [[nodiscard]] static constexpr player_component_type get_player_component_type()
        {
            return player_component_type::time;
        }

        [[nodiscard]] void_awaitable_type on_load_db() override;

        [[nodiscard]] void_awaitable_type save_db() override;

        [[nodiscard]] bool is_modify() const override;

    private:
        using optional_user_server_roles = std::optional<user_server_roles>;

        optional_user_server_roles user_server_roles_;
    };
}
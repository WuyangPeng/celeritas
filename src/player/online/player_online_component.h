#pragma once

#include "player/component/player_component.h"
#include "player/component/player_component_type.h"
#include "database/generated/mysql/auth/account_last_login.h"

#include <optional>

namespace celeritas
{
    class player_online_component final : public player_component
    {
    public:
        using class_type = player_online_component;
        using base_type = player_component;

        explicit player_online_component(player_state* player_state) noexcept;

        [[nodiscard]] static constexpr player_component_type get_player_component_type()
        {
            return player_component_type::online;
        }

        [[nodiscard]] void_awaitable_type on_load_db() override;

        [[nodiscard]] void_awaitable_type save_db() override;

        [[nodiscard]] bool is_modify() const override;

    private:
        using optional_account_last_login = std::optional<account_last_login>;

        optional_account_last_login account_last_login_;
    };
}
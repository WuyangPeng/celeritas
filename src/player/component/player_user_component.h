#pragma once

#include "player_component.h"
#include "player_component_type.h"
#include "database/generated/mysql/player/user.h"

namespace celeritas
{
    class player_user_component final : public player_component
    {
    public:
        using class_type = player_user_component;
        using base_type = player_component;

        explicit player_user_component(user user, player_state* player_state) noexcept;

        [[nodiscard]] static constexpr player_component_type get_player_component_type()
        {
            return player_component_type::user;
        }

        [[nodiscard]] virtual void_awaitable_type on_db_analysis();

        [[nodiscard]] void_awaitable_type save_db() override;

        [[nodiscard]] int64_t get_user_id() const noexcept;

        [[nodiscard]] std::string get_game_server_id() const;

    private:
        user user_;
    };
}
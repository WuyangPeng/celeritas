#pragma once

#include "database/document/player_time_refresh.h"
#include "database/generated/mongo/auth/user_server_roles.h"
#include "database/generated/mongo/player/user_time_refresh.h"
#include "detail/player_time_internal_fwd.h"
#include "player/component/player_component.h"
#include "player/component/player_component_type.h"

namespace celeritas
{
    class player_time_component final : public player_component
    {
    public:
        using class_type = player_time_component;
        using base_type = player_component;

        explicit player_time_component(player_state* player_state) noexcept;

        [[nodiscard]] static constexpr player_component_type get_player_component_type()
        {
            return player_component_type::time;
        }

        [[nodiscard]] void_awaitable_type on_load_db() override;

        [[nodiscard]] void_awaitable_type on_dependencies_ready() override;

        [[nodiscard]] void_awaitable_type save_db() override;

        [[nodiscard]] bool is_modify() const override;

        [[nodiscard]] void_awaitable_type time_callback();

        [[nodiscard]] void_awaitable_type time_callback(time_refresh_type time_refresh_type, int64_t parameter, bool is_login) override;

        void register_timer(player_component_type player_component, time_refresh_type time_refresh_type, int64_t parameter);

        void remove_timer(player_component_type player_component, time_refresh_type time_refresh_type, int64_t parameter);

        [[nodiscard]] int64_t get_next_refresh_time() const;

    private:
        using optional_user_time_refresh = std::optional<user_time_refresh>;
        using player_time_refresh_container = std::vector<player_time_refresh>;
        using player_timer_shared_ptr = std::shared_ptr<player_timer>;

        void calculate_next_refresh_time();

        [[nodiscard]] void_awaitable_type do_time_callback();

        void wait_for_next_tick();

        void init_player_timer(int64_t current_milliseconds);

        optional_user_time_refresh user_time_refresh_;
        player_time_refresh_container player_time_refresh_;
        int64_t next_refresh_time_;
        player_timer_shared_ptr player_timer_;
    };
}
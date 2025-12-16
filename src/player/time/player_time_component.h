#pragma once

#include "database/document/player_time_refresh.h"
#include "detail/player_time_database.h"
#include "detail/player_time_refresh_key.h"
#include "detail/player_time_scheduler.h"
#include "player/component/player_component.h"
#include "player/component/player_component_type.h"

#include <map>

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

        [[nodiscard]] int64_t calculate_next_refresh_time() const;

    private:
        using player_time_refresh_container = std::map<player_time_refresh_key, player_time_refresh>;

        void on_data_change();

        [[nodiscard]] void_awaitable_type on_all_time_callback(bool is_login);

        player_time_refresh_container player_time_refresh_;
        player_time_database database_;
        player_time_scheduler scheduler_;
    };
}

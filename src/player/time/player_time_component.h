#pragma once

#include "detail/player_time_database.h"
#include "detail/player_time_document.h"
#include "detail/player_time_scheduler.h"
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

        [[nodiscard]] void_awaitable_type time_callback(const player_time_refresh_key& player_time_refresh_key, bool is_login) override;

        void register_timer(player_component_type player_component, const player_time_refresh_key& player_time_refresh_key);

        void remove_timer(player_component_type player_component, const player_time_refresh_key& player_time_refresh_key);

    private:
        void on_data_change();

        [[nodiscard]] void_awaitable_type on_all_time_callback(bool is_login);

        player_time_document document_;
        player_time_database database_;
        player_time_scheduler scheduler_;
    };
}

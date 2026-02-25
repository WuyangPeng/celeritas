#pragma once

#include "detail/player_develop_database.h"
#include "detail/player_develop_document.h"
#include "player/component/player_component.h"
#include "player/component/player_component_type.h"

namespace celeritas
{
    class player_develop_component final : public player_component
    {
    public:
        using class_type = player_develop_component;
        using base_type = player_component;
        using optional_develop_data = std::optional<develop_data>;
        using const_item_config_shared_ptr = std::shared_ptr<const config::game::item_config>;
        using optional_develop_data_awaitable = boost::asio::awaitable<optional_develop_data>;

        explicit player_develop_component(player_state* player_state) noexcept;

        [[nodiscard]] static constexpr player_component_type get_player_component_type()
        {
            return player_component_type::develop;
        }

        [[nodiscard]] void_awaitable_type on_load_db() override;

        [[nodiscard]] void_awaitable_type save_db() override;

        [[nodiscard]] bool is_modify() const override;

        [[nodiscard]] game_error_type develop_level(const develop_data& develop_data);

        [[nodiscard]] optional_develop_data_awaitable develop_level(const const_app_config_shared_ptr& app_config, const const_item_config_shared_ptr& item_config);

        [[nodiscard]] game_error_type develop_reset(const develop_data_key& key);

        [[nodiscard]] void_awaitable_type send_initial_sync() override;

        void send_level_message(int rpc, const develop_data& develop_data);

        void send_reset_message(int rpc, const develop_data& develop_data);

        [[nodiscard]] optional_develop_data get_develop_data(const develop_data_key& key) const;

    private:
        void update_document();

        player_develop_database database_;
        player_develop_document document_;
    };
}


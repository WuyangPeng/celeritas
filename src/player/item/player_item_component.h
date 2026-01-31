#pragma once

#include "detail/player_item_database.h"
#include "detail/player_item_document.h"
#include "detail/player_item_selected_database.h"
#include "detail/player_item_selected_document.h"
#include "player/component/player_component.h"
#include "player/component/player_component_type.h"

namespace celeritas
{
    class player_item_component final : public player_component
    {
    public:
        using class_type = player_item_component;
        using base_type = player_component;
        using optional_item_selected_data = std::optional<item_selected_data>;

        explicit player_item_component(player_state* player_state) noexcept;

        [[nodiscard]] static constexpr player_component_type get_player_component_type()
        {
            return player_component_type::item;
        }

        [[nodiscard]] void_awaitable_type on_load_db() override;

        [[nodiscard]] void_awaitable_type on_db_analysis(const const_app_config_shared_ptr& app_config) override;

        [[nodiscard]] void_awaitable_type save_db() override;

        [[nodiscard]] bool is_modify() const override;

        void change_item(const const_app_config_shared_ptr& app_config, int template_id, int64_t count);

        [[nodiscard]] bool can_consume_item(int template_id, int64_t count) const;

        [[nodiscard]] int64_t get_count(int template_id) const;

        [[nodiscard]] bool can_consume_item(const item_container& item) const;

        void change_item(const const_app_config_shared_ptr& app_config, const item_container& item, bool is_login = false);

        [[nodiscard]] optional_item_selected_data change_item_selected(const const_app_config_shared_ptr& app_config,
                                                                       config::item_type item_type,
                                                                       config::item_selected_child_type child_type,
                                                                       int64_t operation_id,
                                                                       int parameter,
                                                                       int64_t selected_id);

        [[nodiscard]] void_awaitable_type on_dependencies_ready() override;

        void send_item_message(bool is_login, int rpc, const item_selected_data& item_selected);

        [[nodiscard]] bool has_item(int64_t item_id) const;

    private:
        using delete_item_id_container = std::vector<int64_t>;

        void update_document();

        player_item_database database_;
        player_item_document document_;
        player_item_selected_database selected_database_;
        player_item_selected_document selected_document_;
    };
}
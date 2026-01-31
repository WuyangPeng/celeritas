#pragma once

#include "item_selected_key.h"
#include "config/config_fwd.h"
#include "config/luban/generated/schema.h"
#include "database/basic/database_data_type_traits.h"
#include "database/document/item_selected_data.h"
#include "player/component/player_state.h"

#include <boost/asio/awaitable.hpp>

#include <map>

namespace celeritas
{
    class player_item_selected_document
    {
    public:
        using class_type = player_item_selected_document;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using const_app_config_shared_ptr = std::shared_ptr<const app_config>;
        using item_selected_data_container = std::map<item_selected_key, item_selected_data>;
        using optional_item_selected_data = std::optional<item_selected_data>;

        explicit player_item_selected_document(player_state* player_state);

        void set_item_selected(traits::param_type::document_array_type item_selected_document);

        [[nodiscard]] traits::document_array_type get_item_selected() const;

        [[nodiscard]] optional_item_selected_data change_item_selected(const const_app_config_shared_ptr& app_config, config::item_type item_type, config::item_selected_child_type child_type, int64_t operation_id, int parameter, int64_t selected_id);

        void on_dependencies_ready();

        void send_item_message(bool is_login, int rpc, const item_selected_data_container& item_selected);

    private:
        void add_item_selected_data(const item_selected_data& item_selected);

        item_selected_data_container item_selected_;
        player_state* player_state_;
    };
}
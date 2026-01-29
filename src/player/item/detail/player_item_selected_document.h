#pragma once

#include "item_selected_key.h"
#include "config/config_fwd.h"
#include "config/luban/generated/schema.h"
#include "database/basic/database_data_type_traits.h"
#include "database/document/item_selected_data.h"

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

        void set_item_selected(traits::param_type::document_array_type item_selected_document);

        [[nodiscard]] traits::document_array_type get_item_selected() const;

        [[nodiscard]] bool change_item_selected(const const_app_config_shared_ptr& app_config, config::item_type item_type, config::item_selected_child_type child_type, int64_t operation_id, int parameter, int64_t selected_id);

    private:
        using item_selected_data_container = std::map<item_selected_key, item_selected_data>;

        void add_item_selected_data(const item_selected_data& item_selected);

        item_selected_data_container item_selected_;
    };
}
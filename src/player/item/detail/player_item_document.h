#pragma once

#include "config/config_fwd.h"
#include "database/database_data_type_traits.h"
#include "database/document/inventory_data.h"
#include "player/item/item_container.h"

#include <boost/asio/awaitable.hpp>

#include <map>

namespace celeritas
{
    class player_item_document
    {
    public:
        using class_type = player_item_document;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using const_app_config_shared_ptr = std::shared_ptr<const app_config>;

        void set_item(traits::param_type::document_array_type item_document);

        [[nodiscard]] traits::document_array_type get_item() const;

        [[nodiscard]] bool change_item(const const_app_config_shared_ptr& app_config, int template_id, int64_t count);

        [[nodiscard]] bool can_consume_item(int template_id, int64_t count) const;

        [[nodiscard]] int64_t get_count(int template_id) const;

        [[nodiscard]] bool can_consume_item(const item_container& item) const;

        [[nodiscard]] bool change_item(const const_app_config_shared_ptr& app_config, const item_container& item);

    private:
        using inventory_data_container = std::map<int64_t, inventory_data>;
        using id_container = std::vector<int64_t>;
        using template_container = std::map<int, id_container>;
        using position_container = std::vector<int64_t>;

        [[nodiscard]] int get_next_position(bool is_squares) const;

        void add_inventory_data(const inventory_data& inventory_data);

        inventory_data_container inventory_data_;
        template_container template_data_;
        position_container position_data_;
    };
}
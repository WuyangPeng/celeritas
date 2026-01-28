#pragma once

#include "database/generated/mongo/player/user_item_selected.h"
#include "player/player_fwd.h"

#include <boost/asio/awaitable.hpp>

#include <optional>

namespace celeritas
{
    class player_item_selected_database
    {
    public:
        using class_type = player_item_selected_database;
        using void_awaitable_type = boost::asio::awaitable<void>;

        player_item_selected_database(player_state* player_state, player_item_component* player_item_component);

        [[nodiscard]] void_awaitable_type load_user_item();

        [[nodiscard]] traits::document_array_type get_item_selected_data() const;

        void set_item(traits::param_type::document_array_type item_selected);

        [[nodiscard]] void_awaitable_type save_db();

        [[nodiscard]] bool is_modify() const;

    private:
        using optional_user_item_selected = std::optional<user_item_selected>;

        player_state* player_state_;
        player_item_component* player_item_component_;
        optional_user_item_selected user_item_selected_;
    };
}
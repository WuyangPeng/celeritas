#pragma once

#include "database/generated/mongo/player/user_red_dots.h"
#include "player/player_fwd.h"

#include <boost/asio/awaitable.hpp>

namespace celeritas
{
    class player_red_dot_database
    {
    public:
        using class_type = player_red_dot_database;
        using void_awaitable_type = boost::asio::awaitable<void>;

        player_red_dot_database(player_state* player_state, player_red_dot_component* player_red_dot_component);

        [[nodiscard]] void_awaitable_type load_user_red_dots();

        [[nodiscard]] traits::document_array_type get_red_dots() const;

        void set_red_dots(traits::param_type::document_array_type red_dots);

    private:
        using optional_user_red_dots = std::optional<user_red_dots>;

        player_state* player_state_;
        player_red_dot_component* player_red_dot_component_;
        optional_user_red_dots user_red_dots_;
    };
}
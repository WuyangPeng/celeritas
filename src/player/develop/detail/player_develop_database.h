#pragma once

#include "database/generated/mongo/player/user_develop.h"
#include "player/player_fwd.h"

#include <boost/asio/awaitable.hpp>

#include <optional>

namespace celeritas
{
    class player_develop_database
    {
    public:
        using class_type = player_develop_database;
        using void_awaitable_type = boost::asio::awaitable<void>;

        player_develop_database(player_state* player_state, player_develop_component* player_develop_component);

        [[nodiscard]] void_awaitable_type load_user_develop();

        [[nodiscard]] traits::document_array_type get_develop_data() const;

        void set_develop(traits::param_type::document_array_type develop);

        [[nodiscard]] void_awaitable_type save_db();

        [[nodiscard]] bool is_modify() const;

    private:
        using optional_user_develop = std::optional<user_develop>;

        player_state* player_state_;
        player_develop_component* player_develop_component_;
        optional_user_develop user_develop_;
    };
}
#pragma once

#include "player_time_document.h"
#include "player_time_refresh_key.h"
#include "database/document/player_time_refresh.h"
#include "database/generated/mongo/player/user_time_refresh.h"

#include <boost/asio/awaitable.hpp>

#include <map>
#include <optional>

namespace celeritas
{
    class player_time_database
    {
    public:
        using class_type = player_time_database;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using player_time_refresh_container = std::map<player_time_refresh_key, player_time_refresh>;
        using player_time_refresh_container_awaitable_type = boost::asio::awaitable<player_time_refresh_container>;

        explicit player_time_database(int64_t user_id, player_time_component* player_time_component, player_time_document* player_time_document);

        [[nodiscard]] player_time_refresh_container_awaitable_type load();

        void update_document();

        [[nodiscard]] void_awaitable_type save();

        [[nodiscard]] bool is_must_save() const;

    private:
        using optional_user_time_refresh = std::optional<user_time_refresh>;

        int64_t user_id_;
        player_time_component* player_time_component_;
        player_time_document* player_time_document_;
        optional_user_time_refresh user_time_refresh_;
    };
}

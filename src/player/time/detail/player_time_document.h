#pragma once

#include "player_time_refresh_key.h"
#include "database/document/player_time_refresh.h"

#include <boost/asio/awaitable.hpp>

#include <map>

namespace celeritas
{
    class player_time_document
    {
    public:
        using class_type = player_time_document;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using player_time_refresh_container = std::map<player_time_refresh_key, player_time_refresh>;
        using function_type = std::function<void()>;

        [[nodiscard]] const player_time_refresh_container& get_player_time_refresh_container() const;

        [[nodiscard]] player_time_refresh_container& get_player_time_refresh_container();

        void set_document(const player_time_refresh_container& player_time_refresh);

        [[nodiscard]] int64_t calculate_next_refresh_time() const;

        [[nodiscard]] bool register_timer(player_component_type component_type, time_refresh_type refresh_type, int64_t parameter);

    private:
        player_time_refresh_container player_time_refresh_;
    };
}
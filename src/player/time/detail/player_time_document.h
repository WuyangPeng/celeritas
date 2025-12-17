#pragma once

#include "database/document/player_time_refresh.h"
#include "player/player_fwd.h"
#include "player/time/player_time_refresh_key.h"

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
        using change_timer_result_awaitable_type = boost::asio::awaitable<change_timer_result>;

        explicit player_time_document(player_state* player_state);

        [[nodiscard]] const player_time_refresh_container& get_player_time_refresh_container() const;

        [[nodiscard]] player_time_refresh_container& get_player_time_refresh_container();

        void set_document(player_time_refresh_container player_time_refresh);

        [[nodiscard]] int64_t calculate_next_refresh_time() const;

        [[nodiscard]] change_timer_result register_timer(player_component_type component_type, const player_time_refresh_key& player_time_refresh_key);

        [[nodiscard]] change_timer_result remove_timer(player_component_type component_type, const player_time_refresh_key& player_time_refresh_key);

        [[nodiscard]] change_timer_result_awaitable_type on_time_callback(bool is_login, bool is_including_default);

        [[nodiscard]] change_timer_result_awaitable_type on_time_callback(const player_time_refresh_key& player_time_refresh_key, bool is_login);

    private:
        player_state* player_state_;
        player_time_refresh_container player_time_refresh_;
    };
}
#pragma once

#include "player_time_internal_fwd.h"
#include "database/document/player_time_refresh.h"
#include "player/player_fwd.h"

#include <boost/asio/awaitable.hpp>

#include <map>
#include <memory>

namespace celeritas
{
    class player_time_scheduler : public std::enable_shared_from_this<player_time_scheduler>
    {
    public:
        using class_type = player_time_scheduler;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using player_time_refresh_container = std::map<player_time_refresh_key, player_time_refresh>;
        using function_type = std::function<void()>;

        player_time_scheduler(player_state* player_state, player_time_component* time_component, player_time_document* player_time_document);

        void register_timer(player_component_type component_type, const player_time_refresh_key& player_time_refresh_key, const function_type& on_change);

        void remove_timer(player_component_type component_type, const player_time_refresh_key& player_time_refresh_key, const function_type& on_change);

        [[nodiscard]] void_awaitable_type on_time_callback(const function_type& on_change);

        [[nodiscard]] void_awaitable_type on_time_callback(const player_time_refresh_key& player_time_refresh_key, bool is_login, const function_type& on_change);

        void wait_for_next_tick();

        void init_player_timer();

        [[nodiscard]] int64_t get_next_refresh_time() const;

        void calculate_next_refresh_time();

        void stop_timer() const;

    private:
        using player_timer_shared_ptr = std::shared_ptr<player_timer>;

        [[nodiscard]] void_awaitable_type do_time_callback(const function_type& on_change);

        void change_player_timer();

        [[nodiscard]] void_awaitable_type do_on_time_callback(const function_type& on_change);

        player_state* player_state_;
        player_time_component* time_component_;
        player_time_document* player_time_document_;
        int64_t next_refresh_time_;
        player_timer_shared_ptr player_timer_;
    };
}

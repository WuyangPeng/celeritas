#pragma once

#include "player_event.h"
#include "player_event_listener.h"

#include <boost/asio/awaitable.hpp>

#include <memory>
#include <unordered_map>
#include <vector>

namespace celeritas
{
    // 事件分发器
    class player_event_dispatcher
    {
    public:
        using class_type = player_event_dispatcher;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using player_event_shared_ptr = std::shared_ptr<player_event>;
        using player_event_listener_shared_ptr = std::shared_ptr<player_event_listener>;
        using listener_id_type = uint64_t;

        player_event_dispatcher() = default;

        // 注册监听器
        [[nodiscard]] listener_id_type register_listener(player_event_type event_type, const player_event_listener_shared_ptr& listener);

        // 注销监听器
        void unregister_listener(player_event_type event_type, listener_id_type listener_id);

        // 分发事件
        [[nodiscard]] void_awaitable_type dispatch(const player_event_shared_ptr& event);

        // 清空所有监听器
        void clear();

    private:
        using listener_pair = std::pair<listener_id_type, player_event_listener_shared_ptr>;
        using listener_vector = std::vector<listener_pair>;
        using listener_map = std::unordered_map<player_event_type, listener_vector>;

        listener_map listeners_;
        listener_id_type next_listener_id_ = 1;
    };
}

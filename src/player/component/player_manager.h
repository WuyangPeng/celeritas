#pragma once

#include "common/common_fwd.h"
#include "database/generated/mysql/player/user.h"
#include "player/player_fwd.h"

#include <boost/asio/awaitable.hpp>

#include <memory>
#include <shared_mutex>
#include <unordered_map>

namespace celeritas
{
    class player_manager
    {
    public:
        using class_type = player_manager;
        using player_state_shared_ptr = std::shared_ptr<player_state>;
        using resource_loader_shared_ptr = std::shared_ptr<resource_loader_base>;
        using void_awaitable_type = boost::asio::awaitable<void>;

        [[nodiscard]] static player_manager& get_instance();

        [[nodiscard]] player_state_shared_ptr add_player(const user& user, const resource_loader_shared_ptr& resource_loader, const std::string& instance_id);

        [[nodiscard]] player_state_shared_ptr get_player(int64_t user_id);

        [[nodiscard]] void_awaitable_type save_db();

        [[nodiscard]] void_awaitable_type time_callback(default_time_type default_time_type);

    private:
        using container_type = std::unordered_map<int64_t, player_state_shared_ptr>;

        player_manager();

        container_type container_;
        std::shared_mutex mutex_;
    };
}
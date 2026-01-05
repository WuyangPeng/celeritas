#pragma once

#include "common/common_fwd.h"
#include "database/generated/mysql/player/user.h"
#include "player/player_fwd.h"
#include "proto/service/player.pb.h"

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
        using io_context_type = boost::asio::io_context;
        using service_login_request_type = proto::service::service_login_request;
        using any_io_executor = boost::asio::any_io_executor;

        [[nodiscard]] static player_manager& get_instance();

        [[nodiscard]] player_state_shared_ptr add_player(const user& user,
                                                         const resource_loader_shared_ptr& resource_loader,
                                                         const any_io_executor& any_io_executor,
                                                         const std::string& instance_id,
                                                         const service_login_request_type& login);

        [[nodiscard]] player_state_shared_ptr get_player(int64_t user_id);

        [[nodiscard]] void_awaitable_type save_db();

        [[nodiscard]] void_awaitable_type time_callback(time_refresh_type time_refresh_type, int64_t parameter);

        [[nodiscard]] void_awaitable_type offline_player(int64_t user_id);

        [[nodiscard]] void_awaitable_type check_player();

        [[nodiscard]] void_awaitable_type clear();

    private:
        using container_type = std::unordered_map<int64_t, player_state_shared_ptr>;

        player_manager();

        container_type container_;
        std::shared_mutex mutex_;
    };
}
#pragma once

#include "database/generated/mysql/player/user.h"
#include "message/message_fwd.h"
#include "player/player_fwd.h"

#include <boost/asio/awaitable.hpp>

namespace celeritas
{
    class service_login
    {
    public:
        using class_type = service_login;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using protobuf_handle_parameter_shared_ptr = std::shared_ptr<protobuf_handle_parameter>;

        service_login(protobuf_handle_parameter_shared_ptr protobuf_handle_parameter, const proto::service::service_login_request& login);

        [[nodiscard]] void_awaitable_type send_message() const;

    private:
        using optional_user = std::optional<user>;
        using optional_user_awaitable_type = boost::asio::awaitable<optional_user>;
        using player_state_shared_ptr = std::shared_ptr<player_state>;

        void send_error_message(game_error_type game_error_type) const;

        void send_success_message(int64_t user_id) const;

        [[nodiscard]] optional_user_awaitable_type get_user() const;

        [[nodiscard]] void_awaitable_type load_player(const optional_user& user, const player_state_shared_ptr& player) const;

        protobuf_handle_parameter_shared_ptr protobuf_handle_parameter_;
        const proto::service::service_login_request& login_;
    };
}
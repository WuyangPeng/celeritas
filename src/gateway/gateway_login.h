#pragma once

#include "database/generated/redis/auth/session_token.h"
#include "message/message_fwd.h"
#include "message/protobuf_handle_parameter.h"
#include "proto/client/player/login.pb.h"

#include <boost/asio/awaitable.hpp>

namespace celeritas
{
    class gateway_login
    {
    public:
        using class_type = gateway_login;
        using void_awaitable_type = boost::asio::awaitable<void>;

        gateway_login(protobuf_handle_parameter protobuf_handle_parameter, const proto::client::login_request& login);

        [[nodiscard]] void_awaitable_type send_message() const;

    private:
        void send_error_message(game_error_type game_error_type) const;

        void write_to_server(const session_token& session_token, const std::string& instance_id, bool new_game_server_id) const;

        protobuf_handle_parameter protobuf_handle_parameter_;
        const proto::client::login_request& login_;
    };
}

#pragma once

#include "database/generated/mysql/player/user.h"
#include "message/protobuf_handle_parameter.h"
#include "proto/service/player.pb.h"

#include <boost/asio/awaitable.hpp>

namespace celeritas
{
    class create_user
    {
    public:
        using class_type = create_user;
        using optional_user = std::optional<user>;
        using optional_user_awaitable_type = boost::asio::awaitable<optional_user>;

        create_user(protobuf_handle_parameter protobuf_handle_parameter, const proto::service::service_login_request& login);

        [[nodiscard]] optional_user_awaitable_type send_message() const;

    private:
        protobuf_handle_parameter protobuf_handle_parameter_;
        const proto::service::service_login_request& login_;
    };
}
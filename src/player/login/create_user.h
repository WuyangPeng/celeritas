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
        using protobuf_handle_parameter_shared_ptr = std::shared_ptr<protobuf_handle_parameter>;

        create_user(protobuf_handle_parameter_shared_ptr protobuf_handle_parameter, const proto::service::service_login_request& login);

        [[nodiscard]] optional_user_awaitable_type save_database() const;

    private:
        protobuf_handle_parameter_shared_ptr protobuf_handle_parameter_;
        const proto::service::service_login_request& login_;
    };
}
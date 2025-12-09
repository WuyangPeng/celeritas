#pragma once

#include "message/protobuf_handle_parameter.h"
#include "proto/service/player.pb.h"

#include <boost/asio/awaitable.hpp>

namespace celeritas
{
    class service_login
    {
    public:
        using class_type = service_login;
        using void_awaitable_type = boost::asio::awaitable<void>;

        service_login(protobuf_handle_parameter protobuf_handle_parameter, const proto::service::service_login_request& login);

        [[nodiscard]] void_awaitable_type send_message() const;

    private:
        protobuf_handle_parameter protobuf_handle_parameter_;
        const proto::service::service_login_request& login_;
    };
}
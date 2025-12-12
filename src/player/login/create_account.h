#pragma once

#include "message/protobuf_handle_parameter.h"
#include "proto/service/player.pb.h"

#include <boost/asio/awaitable.hpp>

namespace celeritas
{
    class create_account
    {
    public:
        using class_type = create_account;
        using bool_awaitable_type = boost::asio::awaitable<bool>;
        using protobuf_handle_parameter_shared_ptr = std::shared_ptr<protobuf_handle_parameter>;

        create_account(protobuf_handle_parameter_shared_ptr protobuf_handle_parameter, const proto::service::service_login_request& login);

        [[nodiscard]] bool_awaitable_type save_database() const;

    private:
        protobuf_handle_parameter_shared_ptr protobuf_handle_parameter_;
        const proto::service::service_login_request& login_;
    };
}
#pragma once

#include "message/concrete_message_handler.h"
#include "proto/client/player/login.pb.h"

#include <boost/asio.hpp>

namespace celeritas
{
    class gateway_login_request_message_handler final : public concrete_message_handler<proto::client::login_request>
    {
    public:
        using class_type = gateway_login_request_message_handler;
        using base_type = concrete_message_handler;

        gateway_login_request_message_handler();

    protected:
        [[nodiscard]] bool handle_concrete(const protobuf_handle_parameter& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry) override;

    private:
        using void_awaitable_type = boost::asio::awaitable<void>;

        [[nodiscard]] static void_awaitable_type response(protobuf_handle_parameter protobuf_handle_parameter, const message_type& login);
    };
}

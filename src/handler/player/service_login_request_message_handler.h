#pragma once

#include "message/handler_base/concrete_message_handler.h"
#include "proto/service/player.pb.h"

#include <boost/asio.hpp>

namespace celeritas
{
    class service_login_request_message_handler final : public concrete_message_handler<proto::service::service_login_request>
    {
    public:
        using class_type = service_login_request_message_handler;
        using base_type = concrete_message_handler;

        service_login_request_message_handler();

    protected:
        [[nodiscard]] bool handle_concrete(const protobuf_handle_parameter_shared_ptr& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry) override;

    private:
        using void_awaitable_type = boost::asio::awaitable<void>;

        [[nodiscard]] static void_awaitable_type response(protobuf_handle_parameter_shared_ptr protobuf_handle_parameter, const message_type& login);
    };
}

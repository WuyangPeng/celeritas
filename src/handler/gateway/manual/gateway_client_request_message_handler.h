#pragma once

#include "message/concrete_message_handler.h"
#include "proto/client/client_request.pb.h"

namespace celeritas
{
    class gateway_client_request_message_handler final : public concrete_message_handler<proto::client::client_request>
    {
    public:
        using class_type = gateway_client_request_message_handler;
        using base_type = concrete_message_handler;

        gateway_client_request_message_handler();

    protected:
        [[nodiscard]] bool handle_concrete(const protobuf_handle_parameter& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry) override;
    };
}

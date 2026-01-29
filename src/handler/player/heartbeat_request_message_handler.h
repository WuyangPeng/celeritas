#pragma once

#include "manual/player_concrete_message_handler.h"
#include "message/handler_base/concrete_message_handler.h"
#include "proto/client/player/heartbeat.pb.h"

namespace celeritas
{
    class heartbeat_request_message_handler final : public player_concrete_message_handler<proto::client::heartbeat_request>
    {
    public:
        using class_type = heartbeat_request_message_handler;
        using base_type = concrete_message_handler;

        heartbeat_request_message_handler() = default;

    protected:
        [[nodiscard]] bool handle_concrete(const protobuf_handle_parameter_shared_ptr& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry) override;
    };
}

#pragma once

#include "manual/player_concrete_message_handler.h"
#include "proto/client/player/develop.pb.h"

namespace celeritas
{
    class develop_reset_message_handler final : public player_concrete_message_handler<proto::client::develop_reset_request>
    {
    public:
        using class_type = develop_reset_message_handler;
        using base_type = player_concrete_message_handler;

        develop_reset_message_handler() = default;

    protected:
        [[nodiscard]] bool handle_concrete(const protobuf_handle_parameter_shared_ptr& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry) override;
    };
}

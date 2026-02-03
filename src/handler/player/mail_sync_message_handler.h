#pragma once

#include "manual/player_concrete_message_handler.h"
#include "proto/client/player/mail.pb.h"

namespace celeritas
{
    class mail_sync_message_handler final : public player_concrete_message_handler<proto::client::mail_request>
    {
    public:
        using class_type = mail_sync_message_handler;
        using base_type = player_concrete_message_handler;

        mail_sync_message_handler() = default;

    protected:
        [[nodiscard]] bool handle_concrete(const protobuf_handle_parameter_shared_ptr& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry) override;
    };
}
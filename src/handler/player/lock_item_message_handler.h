#pragma once

#include "manual/player_concrete_message_handler.h"
#include "proto/client/player/item.pb.h"

namespace celeritas
{
    class lock_item_message_handler final : public player_concrete_message_handler<proto::client::lock_item_request>
    {
    public:
        using class_type = lock_item_message_handler;
        using base_type = player_concrete_message_handler;

        lock_item_message_handler() = default;

    protected:
        [[nodiscard]] bool handle_concrete(const protobuf_handle_parameter_shared_ptr& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry) override;
    };
}

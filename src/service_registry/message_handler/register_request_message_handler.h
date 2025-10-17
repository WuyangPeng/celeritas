#pragma once

#include "message/concrete_message_handler.h"
#include "proto/service/registry.pb.h"

namespace celeritas
{
    class register_request_message_handler final : public concrete_message_handler<proto::service::register_request>
    {
    public:
        using class_type = register_request_message_handler;
        using base_type = concrete_message_handler;

    protected:
        [[nodiscard]] bool handle_concrete(const handle_parameter& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry) override;
    };
}




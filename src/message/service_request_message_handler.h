#pragma once

#include "concrete_message_handler.h"
#include "proto/service/service_request.pb.h"

namespace celeritas
{
    class service_request_message_handler : public concrete_message_handler<proto::service::service_request>
    {
    public:
        using class_type = service_request_message_handler;
        using base_type = concrete_message_handler;

    protected:
        [[nodiscard]] bool handle_concrete(const header& header, const message_type& current_message, const protobuf_message_shared_ptr& request_message, const message_registry_weak_ptr& message_registry, const session_shared_ptr& session) override;
    };
}

#pragma once

#include "message/concrete_message_handler.h"
#include "proto/service/service_request.pb.h"

namespace celeritas
{
    class service_registry_request_message_handler : public concrete_message_handler<proto::service::service_registry_request>
    {
    public:
        using class_type = service_registry_request_message_handler;
        using base_type = concrete_message_handler;

    protected:
        [[nodiscard]] bool handle_concrete(const header& header, const message_shared_ptr& message, const message_registry_weak_ptr& message_registry) override;
    };
}



#pragma once

#include "message/concrete_message_handler.h"
#include "proto/service/service_request.pb.h"

namespace celeritas
{
    class service_registry_request_message_handler final : public concrete_message_handler<proto::service::service_registry_request>
    {
    public:
        using class_type = service_registry_request_message_handler;
        using base_type = concrete_message_handler;

    protected:
        [[nodiscard]] bool handle_concrete(const handle_parameter& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry) override;

    private:
        [[nodiscard]] static bool handle_server_register(const handle_parameter& handle_parameter, const message_type& current_message, const message_registry_shared_ptr& message_registry);

        [[nodiscard]] static bool handle_server_discover(const handle_parameter& handle_parameter, const message_type& current_message, const message_registry_shared_ptr& message_registry);
    };
}



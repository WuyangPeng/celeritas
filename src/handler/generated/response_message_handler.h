#pragma once

#include "message/concrete_message_handler.h"
#include "proto/response.pb.h"

namespace celeritas
{
    class response_message_handler final : public concrete_message_handler<proto::response>
    {
    public:
        using class_type = response_message_handler;
        using base_type = concrete_message_handler;

        response_message_handler();

    protected:
        [[nodiscard]] bool handle_concrete(const handle_parameter& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry) override;
    };
}

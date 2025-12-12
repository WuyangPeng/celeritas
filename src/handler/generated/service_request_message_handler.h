// 此文件是自动生成，请勿手动修改。

#pragma once

#include "message/concrete_message_handler.h"
#include "proto/service/service_request.pb.h"

namespace celeritas
{
    class service_request_message_handler final : public concrete_message_handler<proto::service::service_request>
    {
    public:
        using class_type = service_request_message_handler;
        using base_type = concrete_message_handler;

        service_request_message_handler();

    protected:
        [[nodiscard]] bool handle_concrete(const protobuf_handle_parameter_shared_ptr& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry) override;
    };
}

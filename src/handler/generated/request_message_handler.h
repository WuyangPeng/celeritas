// 此文件是自动生成，请勿手动修改。

#pragma once

#include "message/concrete_message_handler.h"
#include "proto/request.pb.h"

namespace celeritas
{
    class request_message_handler final : public concrete_message_handler<proto::request>
    {
    public:
        using class_type = request_message_handler;
        using base_type = concrete_message_handler;

        request_message_handler();

    protected:
        [[nodiscard]] bool handle_concrete(const handle_parameter& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry) override;
    };
}

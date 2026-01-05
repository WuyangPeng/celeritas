#pragma once

#include "message/handler_base/protobuf_base_message_handler.h"

namespace celeritas
{
    class mock_protobuf_message_handler final : public protobuf_base_message_handler
    {
    public:
        using class_type = mock_protobuf_message_handler;
        using base_Type = protobuf_base_message_handler;

        [[nodiscard]] std::string get_supported_type_name() const override;

        [[nodiscard]] bool handle(const protobuf_handle_parameter_shared_ptr& handle_parameter, const protobuf_message& current_message, const message_registry_weak_ptr& message_registry) override;

        [[nodiscard]] bool is_was_handled() const noexcept;

    private:
        mutable bool was_handled_ = false;
    };
}
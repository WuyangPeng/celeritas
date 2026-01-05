#pragma once

#include "message/handler_base/concrete_message_handler.h"

#include <google/protobuf/wrappers.pb.h>

namespace celeritas
{
    using test_message_type = google::protobuf::StringValue;

    class mock_payload_handler final : public concrete_message_handler<test_message_type>
    {
    public:
        using class_type = mock_payload_handler;
        using base_type = concrete_message_handler;

        using base_type::add_handler_function;
        using base_type::get_handler_function;
        using base_type::has_handler_function;
        using base_type::handle_forward;

        [[nodiscard]] bool is_concrete_handle_called() const noexcept;

        [[nodiscard]] static bool handler(const protobuf_handle_parameter_shared_ptr& handle_parameter, const message_type& current_message, const message_registry_shared_ptr& message_registry);

    protected:
        [[nodiscard]] bool handle_concrete(const protobuf_handle_parameter_shared_ptr& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry) override;

    private:
        mutable bool concrete_handle_called_ = false;
    };
}
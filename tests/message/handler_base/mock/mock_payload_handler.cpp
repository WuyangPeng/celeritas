#include "mock_payload_handler.h"

bool celeritas::mock_payload_handler::is_concrete_handle_called() const noexcept
{
    return concrete_handle_called_;
}

bool celeritas::mock_payload_handler::handler(const protobuf_handle_parameter_shared_ptr& handle_parameter, const message_type& current_message, const message_registry_shared_ptr& message_registry)
{
    return true;
}

bool celeritas::mock_payload_handler::handle_concrete(const protobuf_handle_parameter_shared_ptr& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    concrete_handle_called_ = true;
    return true;
}
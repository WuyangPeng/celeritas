#include "mock_protobuf_message_handler.h"

std::string celeritas::mock_protobuf_message_handler::get_supported_type_name() const
{
    return "google.protobuf.Any";
}

bool celeritas::mock_protobuf_message_handler::handle(const protobuf_handle_parameter_shared_ptr& handle_parameter, const protobuf_message& current_message, const message_registry_weak_ptr& message_registry)
{
    was_handled_ = true;
    return true;
}

bool celeritas::mock_protobuf_message_handler::is_was_handled() const noexcept
{
    return was_handled_;
}
#include "gateway_service_request_message_handler.h"

celeritas::gateway_service_request_message_handler::gateway_service_request_message_handler()
{
}

bool celeritas::gateway_service_request_message_handler::handle_concrete(const protobuf_handle_parameter& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    return true;
}
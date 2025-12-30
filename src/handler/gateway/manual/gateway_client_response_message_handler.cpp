#include "gateway_client_response_message_handler.h"
#include "common/framework/application_loader_base.h"
#include "message/protobuf_handle_parameter.h"

celeritas::gateway_client_response_message_handler::gateway_client_response_message_handler()
{
}

bool celeritas::gateway_client_response_message_handler::handle_concrete(const protobuf_handle_parameter_shared_ptr& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    handle_parameter->write_to_client(*handle_parameter->get_protobuf_message());
    return true;
}
#include "gateway_request_message_handler.h"
#include "message/concrete_message_handler.tpp"

celeritas::gateway_request_message_handler::gateway_request_message_handler()
{
    add_handler_function(proto::request::PayloadCase::kClient,
                         [](const protobuf_handle_parameter_shared_ptr& handle_parameter,
                            const message_type& current_message,
                            const message_registry_shared_ptr& message_registry) -> bool {
                             return handle_dispatch(handle_parameter, current_message, message_registry, &message_type::client);
                         });
}

bool celeritas::gateway_request_message_handler::handle_concrete(const protobuf_handle_parameter_shared_ptr& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    return handle_forward(handle_parameter, current_message, message_registry);
}
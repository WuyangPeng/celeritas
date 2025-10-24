#include "concrete_message_handler.tpp"
#include "request_message_handler.h"

celeritas::request_message_handler::request_message_handler()
{
    add_handler_function(proto::request::PayloadCase::kService,
                         [](const handle_parameter& handle_parameter,
                            const message_type& current_message,
                            const message_registry_shared_ptr& message_registry) -> bool {
                             return handle_dispatch(handle_parameter, current_message, message_registry, &message_type::service);
                         });
}

bool celeritas::request_message_handler::handle_concrete(const handle_parameter& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    return handle_forward(handle_parameter, current_message, message_registry);
}
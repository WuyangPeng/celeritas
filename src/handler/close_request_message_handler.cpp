#include "close_request_message_handler.h"
#include "message/concrete_message_handler.tpp"
#include "service_registry/service_registry.h"

bool celeritas::close_request_message_handler::handle_concrete(const handle_parameter& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    const auto& instance_id = current_message.instance_id();

    service_registry::remove_instance(instance_id);

    return true;
}
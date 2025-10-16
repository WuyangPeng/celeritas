#include "discover_request_message_handler.h"
#include "message/concrete_message_handler.tpp"
#include "service_registry/service_registry.h"

bool celeritas::discover_request_message_handler::handle_concrete(const header& header, const message_type& current_message, const protobuf_message_shared_ptr& request_message, const message_registry_weak_ptr& message_registry)
{
    const auto services = service_registry::get_services(current_message.service_name());

    return true;
}
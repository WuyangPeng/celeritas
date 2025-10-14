#include "register_request_message_handler.h"
#include "message/concrete_message_handler.tpp"
#include "service_registry/service_registry.h"

bool celeritas::register_request_message_handler::handle_concrete(const header& header, const message_shared_ptr& message, const message_registry_weak_ptr& message_registry)
{
    const service_info service_info{ message->instance_id(), message->service_name(), message->host(), message->port(), message->game_server_id() };
    service_registry::register_service(service_info);

    return true;
}

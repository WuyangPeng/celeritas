#include "register_request_message_handler.h"
#include "message/concrete_message_handler.tpp"
#include "service_registry/service_registry.h"

bool celeritas::register_request_message_handler::handle_concrete(const header& header, const message_type& current_message, const protobuf_message_shared_ptr& request_message, const message_registry_weak_ptr& message_registry, const session_shared_ptr& session)
{
    const service_info service_info{ current_message.instance_id(), current_message.service_name(), current_message.host(), current_message.port(), current_message.game_server_id() };
    service_registry::register_service(service_info);

    return true;
}

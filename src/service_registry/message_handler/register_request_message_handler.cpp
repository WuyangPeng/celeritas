#include "register_request_message_handler.h"
#include "message/concrete_message_handler.tpp"
#include "server/server_fwd.h"
#include "service_registry/service_registry.h"

bool celeritas::register_request_message_handler::handle_concrete(const handle_parameter& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    const service_info service_info{ current_message.instance_id(), current_message.service_name(), current_message.host(), current_message.port(), current_message.game_server_id() };
    service_registry::register_service(service_info);

    handle_parameter.write(service_registry_type.data(), *handle_parameter.get_protobuf_message());

    return true;
}

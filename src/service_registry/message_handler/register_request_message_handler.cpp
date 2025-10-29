#include "register_request_message_handler.h"
#include "message/concrete_message_handler.tpp"
#include "server/server_fwd.h"
#include "service_registry/service_info.h"
#include "service_registry/service_registry.h"

bool celeritas::register_request_message_handler::handle_concrete(const handle_parameter& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    service_info::protocol_port_container protocol_port_container{};
    for (const auto& element : current_message.port())
    {
        protocol_port_container.emplace_back(static_cast<server_network_type>(element.protocol()), element.port());
    }

    const service_info service_info{ current_message.instance_id(),
                                     current_message.service_name(),
                                     current_message.host(),
                                     current_message.game_server_id(),
                                     protocol_port_container };

    service_registry::register_service(service_info);

    handle_parameter.write(service_registry_type.data(), *handle_parameter.get_protobuf_message());

    proto::celeritas response{};
    response.mutable_celeritas_response()->mutable_service()->mutable_registry()->mutable_server_register();

    handle_parameter.write(response);

    return true;
}

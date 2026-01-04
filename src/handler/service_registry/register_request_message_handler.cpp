#include "register_request_message_handler.h"
#include "initializer/initializer_constant.h"
#include "initializer/initializer_fwd.h"
#include "message/handler_base/concrete_message_handler.tpp"
#include "message/parameters/protobuf_handle_parameter.h"
#include "proto/celeritas.pb.h"
#include "service_registry/core/service_registry.h"
#include "service_registry/data/service_info.h"

bool celeritas::register_request_message_handler::handle_concrete(const protobuf_handle_parameter_shared_ptr& handle_parameter,
                                                                  const message_type& current_message,
                                                                  const message_registry_weak_ptr& message_registry)
{
    service_info::protocol_port_container protocol_port_container{};
    for (const auto& element : current_message.port())
    {
        protocol_port_container.emplace_back(static_cast<server_network_type>(element.protocol()), element.port());
    }

    const service_info service_info{ current_message.instance_id(),
                                     current_message.service_name(),
                                     current_message.host(),
                                     current_message.external_host(),
                                     current_message.game_server_id(),
                                     protocol_port_container,
                                     current_message.start_server_time() };

    service_registry::register_service(service_info);

    handle_parameter->set_instance_id(service_info.get_instance_id());
    handle_parameter->write_to_server(service_registry_type.data(), *handle_parameter->get_protobuf_message());

    proto::celeritas response{};
    response.mutable_celeritas_response()->mutable_service()->mutable_registry()->mutable_server_register();

    handle_parameter->write(response);

    return true;
}
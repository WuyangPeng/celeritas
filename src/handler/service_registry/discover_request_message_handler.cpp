#include "discover_request_message_handler.h"
#include "config/basic/server_network_type.h"
#include "message/concrete_message_handler.tpp"
#include "message/protobuf_handle_parameter.h"
#include "proto/celeritas.pb.h"
#include "service_registry/core/service_registry.h"
#include "service_registry/data/service_info.h"

bool celeritas::discover_request_message_handler::handle_concrete(const protobuf_handle_parameter_shared_ptr& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    const auto services = service_registry::get_services(current_message.service_name());

    proto::celeritas response{};
    auto* discover_response = response.mutable_celeritas_response()->mutable_service()->mutable_registry()->mutable_server_discover();

    const auto& container = get_all_server_network_type();
    for (const auto& service : services)
    {
        auto* server_info = discover_response->add_server_info();
        server_info->set_instance_id(service.get_instance_id());
        server_info->set_game_server_id(service.get_game_server_id());
        server_info->set_host(service.get_host());
        server_info->set_external_host(service.get_external_host());

        for (const auto& network_type : container)
        {
            if (const auto port = service.get_port(network_type);
                0 < port)
            {
                auto* protocol_port = server_info->add_port();
                protocol_port->set_protocol(static_cast<int>(network_type));
                protocol_port->set_port(port);
            }
        }

        server_info->set_start_server_time(service.get_start_server_time());
        server_info->set_is_health(service.get_health_check_level_type() == health_check_level_type::health);
    }

    discover_response->set_service_name(current_message.service_name());

    handle_parameter->write(response);

    return true;
}
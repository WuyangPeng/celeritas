#include "discover_response_message_handler.h"
#include "message/concrete_message_handler.tpp"
#include "service_registry/service_info.h"
#include "service_registry/service_registry.h"

bool celeritas::discover_response_message_handler::handle_concrete(const protobuf_handle_parameter& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    for (auto index = 0; index < current_message.server_info_size(); ++index)
    {
        const auto& server_info_message = current_message.server_info(index);

        service_info::protocol_port_container protocol_port_container{};
        for (const auto& element : server_info_message.port())
        {
            protocol_port_container.emplace_back(static_cast<server_network_type>(element.protocol()), element.port());
        }

        service_info service_info{ server_info_message.instance_id(),
                                   current_message.service_name(),
                                   server_info_message.host(),
                                   server_info_message.game_server_id(),
                                   protocol_port_container,
                                   server_info_message.start_server_time() };

        if (!server_info_message.is_health())
        {
            service_info.set_health_check_level_type(health_check_level_type::unhealthy);
        }

        service_registry::register_service(service_info);
    }

    return true;
}
#include "gateway_service_login_response_message_handler.h"
#include "common/resource_loader_base.h"
#include "common/gateway_mapping.h"
#include "message/protobuf_handle_parameter.h"
#include "proto/celeritas.pb.h"

celeritas::gateway_service_login_response_message_handler::gateway_service_login_response_message_handler()
{
}

bool celeritas::gateway_service_login_response_message_handler::handle_concrete(const protobuf_handle_parameter& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    gateway_mapping gateway_mapping{ static_cast<server_network_type>(current_message.protocol()), current_message.session_id(), current_message.instance_id() };

    const auto resource_loader = handle_parameter.get_resource_loader();

    resource_loader->add_gateway_mapping(handle_parameter.get_user_id(), std::move(gateway_mapping));

    proto::celeritas response{};
    auto* login = response.mutable_celeritas_response()->mutable_client()->mutable_player()->mutable_login()->mutable_login();
    login->set_current_time(current_message.current_time());

    handle_parameter.write_to_client(response);

    return true;
}
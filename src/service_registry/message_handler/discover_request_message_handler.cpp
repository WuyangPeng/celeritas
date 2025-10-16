#include "discover_request_message_handler.h"
#include "message/concrete_message_handler.tpp"
#include "network/session.h"
#include "service_registry/service_registry.h"
#include "proto/response.pb.h"

bool celeritas::discover_request_message_handler::handle_concrete(const header& header, const message_type& current_message, const protobuf_message_shared_ptr& request_message, const message_registry_weak_ptr& message_registry, const session_shared_ptr& session)
{
    const auto services = service_registry::get_services(current_message.service_name());

    proto::response response{};
    auto* discover_response = response.mutable_service()->mutable_registry()->mutable_discover();

    for (const auto& service : services)
    {
        auto* end_point = discover_response->add_endpoints();
        end_point->set_host(service.get_host());
        end_point->set_port(service.get_port());
    }

    session->write(header, response);

    return true;
}
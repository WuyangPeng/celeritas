#include "service_heartbeat_request_message_handler.h"
#include "message/parameters/protobuf_handle_parameter.h"
#include "proto/celeritas.pb.h"

bool celeritas::service_heartbeat_request_message_handler::handle_concrete(const protobuf_handle_parameter_shared_ptr& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    proto::celeritas response{};
    response.mutable_celeritas_response()->mutable_service()->mutable_registry()->mutable_service_heartbeat();

    handle_parameter->write_to_response(response);

    return true;
}
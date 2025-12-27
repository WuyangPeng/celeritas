#include "client_heartbeat_request_message_handler.h"
#include "common/time_helper.h"
#include "message/concrete_message_handler.tpp"
#include "message/protobuf_handle_parameter.h"
#include "proto/celeritas.pb.h"

bool celeritas::client_heartbeat_request_message_handler::handle_concrete(const protobuf_handle_parameter_shared_ptr& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    proto::celeritas response{};

    response.mutable_celeritas_response()->mutable_client()->mutable_player()->mutable_heartbeat()->set_server_time(time_helper::get_current_milliseconds());
    handle_parameter->write(response);

    return true;
}
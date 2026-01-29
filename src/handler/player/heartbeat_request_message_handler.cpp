#include "heartbeat_request_message_handler.h"
#include "handler/player/manual/player_concrete_message_handler.tpp"
#include "message/handler_base/concrete_message_handler.tpp"
#include "player/online/heartbeat.h"
#include "proto/celeritas.pb.h"

bool celeritas::heartbeat_request_message_handler::handle_concrete(const protobuf_handle_parameter_shared_ptr& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    player_co_spawn_response<heartbeat>(handle_parameter, current_message, handler_channel, "heartbeat error:");

    return true;
}
#include "debug_message_handler.h"
#include "common/common_constant.h"
#include "handler/player/manual/player_concrete_message_handler.tpp"
#include "player/debug/debug_base.h"

bool celeritas::debug_message_handler::handle_concrete(const protobuf_handle_parameter_shared_ptr& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    player_co_spawn_response<debug_base>(handle_parameter, current_message, handler_channel, "debug error:");

    return true;
}
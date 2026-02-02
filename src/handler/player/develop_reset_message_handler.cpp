#include "develop_reset_message_handler.h"
#include "common/common_constant.h"
#include "handler/player/manual/player_concrete_message_handler.tpp"
#include "player/develop/develop_reset.h"

bool celeritas::develop_reset_message_handler::handle_concrete(const protobuf_handle_parameter_shared_ptr& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    player_co_spawn_response<develop_reset>(handle_parameter, current_message, handler_channel, "develop reset error:");

    return true;
}
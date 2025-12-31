#include "change_role_name_message_handler.h"
#include "message/protobuf_handle_parameter.h"
#include "player/component/player_manager.h"

bool celeritas::change_role_name_message_handler::handle_concrete(const protobuf_handle_parameter_shared_ptr& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    if (const auto player = player_manager::get_instance().get_player(handle_parameter->get_user_id());
        player != nullptr)
    {
    }

    return true;
}
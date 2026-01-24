#include "offline_request_message_handler.h"
#include "message/parameters/protobuf_handle_parameter.h"
#include "player/component/player_manager.h"
#include "player/component/player_state.h"

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>

bool celeritas::offline_request_message_handler::handle_concrete(const protobuf_handle_parameter_shared_ptr& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    const auto user_id = handle_parameter->get_user_id();

    const auto player = player_manager::get_instance().get_player(user_id);

    if (player != nullptr)
    {
        player_manager::get_instance().offline_player(user_id);
    }

    return true;
}
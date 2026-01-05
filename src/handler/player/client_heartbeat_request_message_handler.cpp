#include "client_heartbeat_request_message_handler.h"
#include "common/core/time_helper.h"
#include "message/handler_base/concrete_message_handler.tpp"
#include "message/basic/game_error_type.h"
#include "message/parameters/protobuf_handle_parameter.h"
#include "player/component/player_manager.h"
#include "player/component/player_state.h"
#include "player/online/player_online_component.h"
#include "proto/celeritas.pb.h"
#include "player/component/player_state.tpp"

bool celeritas::client_heartbeat_request_message_handler::handle_concrete(const protobuf_handle_parameter_shared_ptr& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    if (const auto player = player_manager::get_instance().get_player(handle_parameter->get_user_id());
        player != nullptr)
    {
        player->get_component<player_online_component>()->set_heartbeat();
    }

    proto::celeritas response{};

    response.mutable_celeritas_response()->mutable_client()->mutable_player()->mutable_heartbeat()->set_server_time(time_helper::get_current_milliseconds());
    handle_parameter->write_to_response(response);

    return true;
}
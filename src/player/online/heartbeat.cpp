#include "heartbeat.h"
#include "common/core/time_helper.h"
#include "common/logging/logger.h"
#include "initializer/initializer_constant.h"
#include "player/online/player_online_component.h"
#include "proto/celeritas.pb.h"

celeritas::heartbeat::heartbeat(protobuf_handle_parameter_shared_ptr handle_parameter, player_state_shared_ptr player_state, request_type request)
    : base_type{ std::move(handle_parameter), std::move(player_state) }
{
}

celeritas::player_service_base::void_awaitable_type celeritas::heartbeat::response()
{
    get_player_state()->get_component<player_online_component>()->set_heartbeat();

    co_return;
}

void celeritas::heartbeat::send_heartbeat()
{
    const header header{ get_rpc(), get_player_state()->get_user_id() };
    proto::celeritas response{};

    response.mutable_celeritas_response()->mutable_client()->mutable_player()->mutable_heartbeat()->mutable_heartbeat()->set_server_time(time_helper::get_current_milliseconds());
    if (!get_player_state()->write(gateway_type.data(), get_player_state()->get_instance_id(), header, response))
    {
        LOG_CHANNEL(player_channel, error) << "send message error.";
    }
}
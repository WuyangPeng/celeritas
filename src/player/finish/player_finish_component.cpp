#include "player_finish_component.h"
#include "common/logger.h"
#include "initializer/initializer_fwd.h"
#include "player/component/player_component.h"
#include "player/component/player_state.h"
#include "proto/celeritas.pb.h"

celeritas::player_finish_component::player_finish_component(player_state* player_state) noexcept
    : base_type{ get_player_component_type(), player_state }
{
}

celeritas::player_component::void_awaitable_type celeritas::player_finish_component::send_initial_sync()
{
    auto* player_state = get_player_state();

    const header header{ player_state->get_user_id() };

    proto::celeritas response{};
    response.mutable_celeritas_response()->mutable_client()->mutable_player()->mutable_login()->mutable_login_finish();

    if (!player_state->write(gateway_type.data(), player_state->get_instance_id(), header, response))
    {
        LOG_CHANNEL(message_channel, error) << "send message error.";
    }

    co_return;
}
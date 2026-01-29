#include "player_debug_component.h"
#include "auth/config/server_cell_repository.h"
#include "auth/config/server_mode_type.h"
#include "auth/server_list/login_servers.h"
#include "common/core/enum_cast.h"
#include "common/logging/logger.h"
#include "initializer/initializer_constant.h"
#include "message/basic/header.h"
#include "player/component/player_state.h"
#include "player/user/player_user_component.h"
#include "proto/celeritas.pb.h"

celeritas::player_debug_component::player_debug_component(player_state* player_state) noexcept
    : base_type{ get_player_component_type(), player_state }, is_debug_{ false }
{
}

celeritas::player_component::void_awaitable_type celeritas::player_debug_component::on_db_analysis()
{
    const auto game_server_id = get_player_state()->get_game_server_id();
    if (const auto optional_server_cell = server_cell_repository::get_instance().get_server_cell(game_server_id))
    {
        if (const auto& server_cell = *optional_server_cell;
            server_cell.get_mode() == enum_cast_underlying(server_mode_type::test))
        {
            is_debug_ = true;
        }
        else if (server_cell.get_mode() == enum_cast_underlying(server_mode_type::whitelist))
        {
            if (const auto user_component = get_player_state()->get_component<player_user_component>();
                user_component->get_permission() > 0)
            {
                is_debug_ = true;
            }
        }
    }

    co_return;
}

celeritas::player_component::void_awaitable_type celeritas::player_debug_component::send_initial_sync()
{
    auto* player_state = get_player_state();

    const header header{ player_state->get_user_id() };

    proto::celeritas response{};
    auto* debug_response = response.mutable_celeritas_response()->mutable_client()->mutable_player()->mutable_debug()->mutable_debug_info();
    debug_response->set_open_debug(is_debug_);

    if (!player_state->write(gateway_type.data(), player_state->get_instance_id(), header, response))
    {
        LOG_CHANNEL(player_channel, error) << "send message error.";
    }

    co_return;
}

bool celeritas::player_debug_component::is_debug() const
{
    return is_debug_;
}


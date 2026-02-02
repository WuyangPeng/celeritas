#include "develop_data_key.h"
#include "develop_reset.h"
#include "player_develop_component.h"
#include "config/game/game_tables.h"
#include "config/game/pretreatment_config.h"
#include "database/document/develop_data.h"

celeritas::develop_reset::develop_reset_shared_ptr celeritas::develop_reset::create(protobuf_handle_parameter_shared_ptr handle_parameter, player_state_shared_ptr player_state, request_type request)
{
    return std::make_shared<class_type>(std::move(handle_parameter), std::move(player_state), std::move(request));
}

celeritas::develop_reset::develop_reset(protobuf_handle_parameter_shared_ptr handle_parameter, player_state_shared_ptr player_state, request_type request)
    : base_type{ std::move(handle_parameter), std::move(player_state) },
      request_{ std::move(request) },
      player_item_component_{ get_player_state()->get_component<player_item_component>() },
      player_develop_component_{ get_player_state()->get_component<player_develop_component>() }
{
}

celeritas::player_service_base::void_awaitable_type celeritas::develop_reset::response()
{
    const auto develop_level_config = get_game_tables()->get_pretreatment_config()->get_develop_level_config();
    const auto& request_develop = request_.develop();

    const develop_data_key develop_data_key{ request_develop.system_id(), request_develop.instance_id() };
    auto optional_develop = player_develop_component_->get_develop_data(develop_data_key);
    if (!optional_develop)
    {
        get_player_state()->send_error_message(get_rpc(), game_error_type::develop_not_exist);
        co_return;
    }
    co_return;
}
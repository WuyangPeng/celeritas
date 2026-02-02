#include "develop_level.h"
#include "player_develop_component.h"
#include "config/game/game_tables.h"
#include "config/game/pretreatment_config.h"

celeritas::develop_level::develop_level_shared_ptr celeritas::develop_level::create(protobuf_handle_parameter_shared_ptr handle_parameter, player_state_shared_ptr player_state, request_type request)
{
    return std::make_shared<class_type>(std::move(handle_parameter), std::move(player_state), std::move(request));
}

celeritas::develop_level::develop_level(protobuf_handle_parameter_shared_ptr handle_parameter, player_state_shared_ptr player_state, request_type request)
    : base_type{ std::move(handle_parameter), std::move(player_state) },
      request_{ std::move(request) },
      player_item_component_{ get_player_state()->get_component<player_item_component>() },
      player_develop_component_{ get_player_state()->get_component<player_develop_component>() }
{
}

celeritas::player_service_base::void_awaitable_type celeritas::develop_level::response()
{
    const auto develop_level_config = get_game_tables()->get_pretreatment_config()->get_develop_level_config();
    const auto& develop = request_.develop();

    const develop_data_key develop_data_key{ develop.system_id(), develop.instance_id() };
    const auto develop_data = player_develop_component_->get_develop_data(develop_data_key);
    if (!develop_data)
    {
        co_return;
    }

    const develop_config_data_key develop_config_data_key{ develop.system_id(), develop.instance_id(), develop_data->get_level() };

    const auto develop_level = develop_level_config->get_develop_level(develop_config_data_key);
    if (!develop_level)
    {
        get_player_state()->send_error_message(get_rpc(), game_error_type::max_develop);
        co_return;
    }
}
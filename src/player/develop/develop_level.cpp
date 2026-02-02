#include "develop_level.h"
#include "player_develop_component.h"
#include "config/game/game_tables.h"
#include "config/game/pretreatment_config.h"
#include "player/component/player_state.tpp"
#include "player/item/item_container.h"
#include "player/item/player_item_component.h"

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
    const auto& request_develop = request_.develop();

    const develop_data_key develop_data_key{ request_develop.system_id(), request_develop.instance_id() };
    auto optional_develop = player_develop_component_->get_develop_data(develop_data_key);
    if (!optional_develop)
    {
        optional_develop = develop_data{ request_develop.system_id(), request_develop.instance_id() };
    }

    auto& develop = *optional_develop;

    auto level = develop.get_level();
    const auto develop_config = get_game_tables()->get_pretreatment_config()->get_develop_config()->get_develop(develop_data_key);
    if (!develop_config)
    {
        get_player_state()->send_error_message(get_rpc(), game_error_type::max_develop);
        co_return;
    }

    item_container container{};
    for (; level < request_develop.level(); ++level)
    {
        const develop_level_data_key develop_config_data_key{ develop.get_system_id(), develop.get_instance_id(), level };

        const auto develop_level = develop_level_config->get_develop_level(develop_config_data_key);
        if (!develop_level || (*develop_config)->maxLevel <= level)
        {
            break;
        }

        for (const auto& player_item : (*develop_level)->playerItem)
        {
            container.add_item_info(player_item->itemId, player_item->itemCount);
        }
    }

    if (level == develop.get_level())
    {
        get_player_state()->send_error_message(get_rpc(), game_error_type::max_develop);
        co_return;
    }

    if (!player_item_component_->can_consume_item(container))
    {
        get_player_state()->send_error_message(get_rpc(), game_error_type::develop_item_not_insufficient);
        co_return;
    }

    player_item_component_->consume_item(get_config(), container);

    develop.set_level(level);

    if (const auto game_error_type = player_develop_component_->develop_level(develop);
        game_error_type == game_error_type::success)
    {
        player_develop_component_->send_level_message(get_rpc(), develop);
    }
    else
    {
        get_player_state()->send_error_message(get_rpc(), game_error_type);
    }
}
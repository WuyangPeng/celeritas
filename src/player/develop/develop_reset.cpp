#include "develop_reset.h"
#include "player_develop_component.h"
#include "config/game/develop_data_key.h"
#include "config/game/game_tables.h"
#include "config/game/pretreatment_config.h"
#include "player/item/item_container.h"
#include "player/item/player_item_component.h"

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
    const auto& request_develop = request_.develop();

    const develop_data_key develop_data_key{ request_develop.system_id(), request_develop.instance_id() };
    auto optional_develop = player_develop_component_->get_develop_data(develop_data_key);
    if (!optional_develop)
    {
        get_player_state()->send_error_message(get_rpc(), game_error_type::develop_not_exist);
        co_return;
    }

    const auto develop_config = get_game_tables()->get_tables()->develop_config_container;
    const auto optional_current_develop_config = develop_config.get(request_develop.system_id());
    if (!optional_current_develop_config)
    {
        get_player_state()->send_error_message(get_rpc(), game_error_type::develop_not_exist);
        co_return;
    }

    const auto& current_develop_config = *optional_current_develop_config;
    const auto develop_reset_type = current_develop_config->developResetType;

    if (develop_reset_type == config::develop_reset_type::non_resettable)
    {
        get_player_state()->send_error_message(get_rpc(), game_error_type::non_resettable);
        co_return;
    }

    auto& develop = *optional_develop;

    const auto level = develop.get_level();
    if (level == 0)
    {
        get_player_state()->send_error_message(get_rpc(), game_error_type::non_resettable);
        co_return;
    }

    const auto develop_level_config = get_game_tables()->get_pretreatment_config()->get_develop_level_config();
    item_container container{};
    for (auto i = 0; i < level; ++i)
    {
        const develop_level_data_key develop_config_data_key{ develop.get_system_id(), i };

        const auto develop_level = develop_level_config->get_develop_level(develop_config_data_key);
        if (!develop_level)
        {
            break;
        }

        for (const auto& player_item : (*develop_level)->playerItem)
        {
            container.add_item_info(player_item->itemId, player_item->itemCount);
        }
    }

    if (develop_reset_type == config::develop_reset_type::refund)
    {
        container.proportion_item(current_develop_config->refundProportion);
    }

    co_await player_item_component_->produce_item(get_config(), container);

    develop.clear();

    if (const auto game_error_type = player_develop_component_->develop_reset(develop_data_key);
        game_error_type == game_error_type::success)
    {
        player_develop_component_->send_reset_message(get_rpc(), develop);
    }
    else
    {
        get_player_state()->send_error_message(get_rpc(), game_error_type);
    }

    co_return;
}
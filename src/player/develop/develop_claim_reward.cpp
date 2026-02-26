#include "develop_claim_reward.h"
#include "player_develop_component.h"
#include "config/game/game_tables.h"
#include "config/game/pretreatment_config.h"
#include "player/component/player_state.tpp"
#include "player/item/item_container.h"
#include "player/item/player_item_component.h"

celeritas::develop_claim_reward::develop_claim_reward_shared_ptr celeritas::develop_claim_reward::create(protobuf_handle_parameter_shared_ptr handle_parameter, player_state_shared_ptr player_state, request_type request)
{
    return std::make_shared<class_type>(std::move(handle_parameter), std::move(player_state), std::move(request));
}

celeritas::develop_claim_reward::develop_claim_reward(protobuf_handle_parameter_shared_ptr handle_parameter, player_state_shared_ptr player_state, request_type request)
    : base_type{ std::move(handle_parameter), std::move(player_state) },
      request_{ std::move(request) },
      player_item_component_{ get_player_state()->get_component<player_item_component>() },
      player_develop_component_{ get_player_state()->get_component<player_develop_component>() }
{
}

celeritas::player_service_base::void_awaitable_type celeritas::develop_claim_reward::response()
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

    auto& develop = *optional_develop;

    const auto current_level = develop.get_level();
    const auto reward_level = develop.get_reward_level();

    if (reward_level >= current_level)
    {
        get_player_state()->send_error_message(get_rpc(), game_error_type::develop_reward_already_claimed);
        co_return;
    }

    if (const auto develop_config = get_game_tables()->get_tables()->develop_config_container.get(request_develop.system_id()); !develop_config)
    {
        get_player_state()->send_error_message(get_rpc(), game_error_type::develop_not_exist);
        co_return;
    }

    item_container container{};
    for (auto level = reward_level; level < current_level; ++level)
    {
        const develop_level_data_key develop_config_data_key{ develop.get_system_id(), level };

        const auto develop_level = develop_level_config->get_develop_level(develop_config_data_key);
        if (!develop_level)
        {
            break;
        }

        for (const auto& reward_item : (*develop_level)->reward)
        {
            container.add_item_info(reward_item->itemId, reward_item->itemCount);
        }
    }

    co_await player_item_component_->produce_item(get_config(), container);

    develop.set_reward_level(current_level);

    if (const auto game_error_type = player_develop_component_->develop_claim_reward(develop);
        game_error_type == game_error_type::success)
    {
        player_develop_component_->send_claim_reward_message(get_rpc(), develop);
    }
    else
    {
        get_player_state()->send_error_message(get_rpc(), game_error_type);
    }
}

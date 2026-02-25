#include "player_item_component.h"
#include "unlock_item.h"
#include "common/logging/logger.h"
#include "config/game/game_config.h"
#include "config/game/game_tables.h"
#include "initializer/initializer_constant.h"
#include "message/basic/header.h"
#include "player/component/player_state.tpp"
#include "player/task/player_task_component.h"
#include "proto/celeritas.pb.h"

celeritas::unlock_item::unlock_item_shared_ptr celeritas::unlock_item::create(protobuf_handle_parameter_shared_ptr handle_parameter, player_state_shared_ptr player_state, request_type request)
{
    return std::make_shared<class_type>(std::move(handle_parameter), std::move(player_state), std::move(request));
}

celeritas::unlock_item::unlock_item(protobuf_handle_parameter_shared_ptr handle_parameter, player_state_shared_ptr player_state, request_type request)
    : base_type{ std::move(handle_parameter), std::move(player_state) },
      request_{ std::move(request) },
      player_item_component_{ get_player_state()->get_component<player_item_component>() }
{
}

celeritas::player_service_base::void_awaitable_type celeritas::unlock_item::response()
{
    const auto template_id = request_.template_id();

    if (player_item_component_->get_count(template_id) > 0)
    {
        get_player_state()->send_error_message(get_rpc(), game_error_type::item_already_exist);
        co_return;
    }

    const auto game_tables = game_config::get_instance().get_game_tables();
    const auto optional_item_config = game_tables->get_tables()->item_config_container.get(template_id);

    if (!optional_item_config)
    {
        get_player_state()->send_error_message(get_rpc(), game_error_type::item_not_exist);
        co_return;
    }

    const auto& item_config = optional_item_config.value();
    const auto unlock_task_id = item_config->unlockTaskId;

    if (unlock_task_id == 0)
    {
        co_await player_item_component_->produce_item(get_config(), template_id, 1);
        send_unlock_item_response();
        co_return;
    }

    const auto optional_task_config = game_tables->get_tables()->task_config_container.get(unlock_task_id);

    if (!optional_task_config)
    {
        get_player_state()->send_error_message(get_rpc(), game_error_type::task_not_exist);
        co_return;
    }

    const auto& task_config = optional_task_config.value();

    const auto task_component = get_player_state()->get_component<player_task_component>();
    const auto current_progress = task_component->get_progress(task_config->taskComponentType, unlock_task_id);

    if (current_progress < task_config->progress)
    {
        get_player_state()->send_error_message(get_rpc(), game_error_type::task_progress_not_enough);
        co_return;
    }

    co_await player_item_component_->produce_item(get_config(), template_id, 1);
    send_unlock_item_response();

    co_return;
}

void celeritas::unlock_item::send_unlock_item_response()
{
    const auto player_state = get_player_state();

    const header header{ get_rpc(), player_state->get_user_id() };

    proto::celeritas response{};
    response.mutable_celeritas_response()->mutable_client()->mutable_player()->mutable_item()->mutable_unlock_item();

    if (!player_state->write(gateway_type.data(), player_state->get_instance_id(), header, response))
    {
        LOG_CHANNEL(player_channel, error) << "send unlock item response error.";
    }
}

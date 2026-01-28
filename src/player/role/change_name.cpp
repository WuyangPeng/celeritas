#include "change_name.h"
#include "player_role_component.h"
#include "config/game/game_config.h"
#include "config/game/game_tables.h"

celeritas::change_name::change_name(protobuf_handle_parameter_shared_ptr handle_parameter, player_state_shared_ptr player_state, const request_type& request)
    : base_type{ std::move(handle_parameter), std::move(player_state) },
      request_{ request },
      player_role_component_{ get_player_state()->get_component<player_role_component>() },
      player_item_component_{ get_player_state()->get_component<player_item_component>() }
{
}

celeritas::player_service_base::void_awaitable_type celeritas::change_name::response()
{
    const auto tables = game_config::get_instance().get_game_tables()->get_tables();
    const auto& optional_rename_cost = get_rename_cost(tables->rename_cost_config_container);
    if (!optional_rename_cost)
    {
        get_player_state()->send_error_message(get_rpc(), game_error_type::insufficient_number_of_name_changes);
        co_return;
    }

    const auto& rename_cost = *optional_rename_cost;

    if (const auto game_error = co_await player_role_component_->change_name(request_.surname(), request_.name());
        game_error == game_error_type::success)
    {
        player_role_component_->send_role_response(get_rpc());
    }
    else
    {
        get_player_state()->send_error_message(get_rpc(), game_error);
    }
}

celeritas::change_name::optional_priority_item_type celeritas::change_name::get_rename_cost(const container_type& container)
{
    const auto change_count = player_role_component_->get_name_change_count();
    for (const auto& item : container.getDataList())
    {
        if (const auto& rename_count = item->renameCount;
            rename_count->min <= change_count)
        {
            if (rename_count->max == 0 || change_count <= rename_count->max)
            {
                return item->item;
            }
        }
    }

    return std::nullopt;
}


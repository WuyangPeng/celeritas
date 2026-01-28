#include "change_name.h"

#include <utility>
#include "player_role_component.h"
#include "config/game/game_config.h"
#include "config/game/game_tables.h"
#include "config/game/pretreatment_config.h"
#include "player/component/player_state.tpp"
#include "player/item/player_item_component.h"

celeritas::change_name::change_name(protobuf_handle_parameter_shared_ptr handle_parameter, player_state_shared_ptr player_state, request_type request)
    : base_type{ std::move(handle_parameter), std::move(player_state) },
      request_{ std::move(request) },
      player_role_component_{ get_player_state()->get_component<player_role_component>() },
      player_item_component_{ get_player_state()->get_component<player_item_component>() }
{
}

celeritas::player_service_base::void_awaitable_type celeritas::change_name::response()
{
    const auto tables = get_game_tables()->get_tables();
    const auto& optional_rename_cost = get_rename_cost(tables->rename_cost_config_container);
    if (!optional_rename_cost)
    {
        get_player_state()->send_error_message(get_rpc(), game_error_type::insufficient_number_of_name_changes);
        co_return;
    }

    const auto& rename_cost = *optional_rename_cost;
    auto optional_cost_item = get_cost_item(rename_cost);
    if (!optional_cost_item)
    {
        get_player_state()->send_error_message(get_rpc(), game_error_type::insufficient_item_of_name_change);
        co_return;
    }

    cost_item(*optional_cost_item);

    if (const auto game_error = co_await player_role_component_->change_name(request_.surname(), request_.name());
        game_error != game_error_type::success)
    {
        get_player_state()->send_error_message(get_rpc(), game_error);
        co_return;
    }

    player_role_component_->send_role_response(get_rpc());
}

celeritas::change_name::optional_priority_item_container_type celeritas::change_name::get_rename_cost(const container_type& container) const
{
    const auto change_count = player_role_component_->get_name_change_count();
    for (const auto& item : container.getDataList())
    {
        if (const auto& rename_count = item->renameCount;
            rename_count->min <= change_count)
        {
            if (rename_count->max == 0 || change_count <= rename_count->max)
            {
                return get_game_tables()->get_pretreatment_config()->get_rename_cost_config()->get_priority_item(item->id);
            }
        }
    }

    return std::nullopt;
}

celeritas::change_name::optional_priority_item_type celeritas::change_name::get_cost_item(const priority_item_container_type& priority_item)
{
    for (const auto& element : priority_item)
    {
        if (player_item_component_->can_consume_item(element->itemId, element->itemCount))
        {
            return element;
        }
    }

    return std::nullopt;
}

void celeritas::change_name::cost_item(const priority_item_type& priority_item)
{
    player_item_component_->change_item(get_config(), priority_item->itemId, priority_item->itemCount);
}



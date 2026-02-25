#include "add_item.h"
#include "config/game/game_config.h"
#include "config/game/game_tables.h"
#include "player/item/player_item_component.h"

celeritas::add_item::add_item(protobuf_handle_parameter_shared_ptr handle_parameter, player_state_shared_ptr player_state, request_type request)
    : base_type{ std::move(handle_parameter), std::move(player_state), std::move(request) },
      player_item_component_{ get_player_state()->get_component<player_item_component>() }
{
}

celeritas::debug_base::game_error_awaitable_type celeritas::add_item::do_response()
{
    const auto id = get_id();
    const auto count = get_parameter();

    if (count == 0)
    {
        co_return game_error_type::item_count_error;
    }

    if (const auto item_config = game_config::get_instance().get_game_tables()->get_tables()->item_config_container.get(id);
        !item_config)
    {
        co_return game_error_type::item_id_error;
    }

    co_await player_item_component_->produce_item(get_config(), id, count);

    co_return game_error_type::success;
}


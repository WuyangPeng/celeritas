#include "add_item.h"
#include "player/item/player_item_component.h"

celeritas::add_item::add_item(protobuf_handle_parameter_shared_ptr handle_parameter, player_state_shared_ptr player_state, request_type request)
    : base_type{ std::move(handle_parameter), std::move(player_state), std::move(request) },
      player_item_component_{ get_player_state()->get_component<player_item_component>() }
{
}

celeritas::debug_base::game_error_awaitable_type celeritas::add_item::do_response()
{
    player_item_component_->change_item(get_config(), get_id(), get_parameter());

    co_return game_error_type::success;
}


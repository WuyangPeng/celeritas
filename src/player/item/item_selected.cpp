#include "item_selected.h"
#include "player_item_component.h"
#include "common/core/enum_cast.h"
#include "message/basic/header.h"
#include "proto/celeritas.pb.h"

celeritas::item_selected::item_selected_shared_ptr celeritas::item_selected::create(protobuf_handle_parameter_shared_ptr handle_parameter, player_state_shared_ptr player_state, request_type request)
{
    return std::make_shared<class_type>(std::move(handle_parameter), std::move(player_state), std::move(request));
}

celeritas::item_selected::item_selected(protobuf_handle_parameter_shared_ptr handle_parameter, player_state_shared_ptr player_state, request_type request)
    : base_type{ std::move(handle_parameter), std::move(player_state) },
      request_{ std::move(request) },
      player_item_component_{ get_player_state()->get_component<player_item_component>() }
{
}

celeritas::player_service_base::void_awaitable_type celeritas::item_selected::response()
{
    const auto& item_selected = request_.item_selected();
    if (item_selected.selected_id() != 0)
    {
        if (!player_item_component_->has_item(item_selected.selected_id()))
        {
            get_player_state()->send_error_message(get_rpc(), game_error_type::item_not_exist);
            co_return;
        }
    }

    const auto optional_item_selected_data = player_item_component_->change_item_selected(get_config(),
                                                                                          underlying_cast_enum<config::item_type>(item_selected.item_type()),
                                                                                          underlying_cast_enum<config::item_selected_child_type>(item_selected.child_type()),
                                                                                          item_selected.operation_id(),
                                                                                          item_selected.parameter(),
                                                                                          item_selected.selected_id());
    if (optional_item_selected_data)
    {
        player_item_component_->send_item_message(false, get_rpc(), *optional_item_selected_data);
    }
    else
    {
        get_player_state()->send_error_message(get_rpc(), game_error_type::item_selected_error);
    }
}
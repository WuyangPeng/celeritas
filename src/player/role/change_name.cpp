#include "change_name.h"
#include "player_role_component.h"

celeritas::change_name::change_name(protobuf_handle_parameter_shared_ptr handle_parameter, player_state_shared_ptr player_state, const request_type& request)
    : base_type{ std::move(handle_parameter), std::move(player_state) },
      request_{ request },
      player_role_component_{ get_player_state()->get_component<player_role_component>() }
{
}

celeritas::player_service_base::void_awaitable_type celeritas::change_name::response()
{
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


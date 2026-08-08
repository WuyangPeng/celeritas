#include "player_item_component.h"
#include "lock_item.h"
#include "common/logging/logger.h"
#include "initializer/initializer_constant.h"
#include "message/basic/header.h"
#include "player/component/player_state.tpp"
#include "proto/celeritas.pb.h"

celeritas::lock_item::lock_item_shared_ptr celeritas::lock_item::create(protobuf_handle_parameter_shared_ptr handle_parameter, player_state_shared_ptr player_state, request_type request)
{
    return std::make_shared<class_type>(std::move(handle_parameter), std::move(player_state), std::move(request));
}

celeritas::lock_item::lock_item(protobuf_handle_parameter_shared_ptr handle_parameter, player_state_shared_ptr player_state, request_type request)
    : base_type{ std::move(handle_parameter), std::move(player_state) },
      request_{ std::move(request) },
      player_item_component_{ get_player_state()->get_component<player_item_component>() }
{
}

celeritas::player_service_base::void_awaitable_type celeritas::lock_item::response()
{
    const auto item_id = request_.item_id();
    const auto is_locked = request_.is_locked();

    if (!player_item_component_->lock_item(item_id, is_locked))
    {
        get_player_state()->send_error_message(get_rpc(), game_error_type::item_not_exist);
        co_return;
    }

    send_lock_item_response();
    co_return;
}

void celeritas::lock_item::send_lock_item_response()
{
    const auto player_state = get_player_state();
    const header header{ get_rpc(), player_state->get_user_id() };

    proto::celeritas response{};
    auto* lock_response = response.mutable_celeritas_response()->mutable_client()->mutable_player()->mutable_item()->mutable_lock_item();
    lock_response->set_item_id(request_.item_id());
    lock_response->set_is_locked(request_.is_locked());

    if (!player_state->write(gateway_type.data(), player_state->get_instance_id(), header, response))
    {
        LOG_CHANNEL(player_channel, error) << "send lock item response error.";
    }
}

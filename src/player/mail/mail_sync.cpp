#include "mail_sync.h"
#include "player_mail_component.h"
#include "common/core/enum_cast.h"

celeritas::mail_sync::mail_sync_shared_ptr celeritas::mail_sync::create(protobuf_handle_parameter_shared_ptr handle_parameter, player_state_shared_ptr player_state, request_type request)
{
    return std::make_shared<class_type>(std::move(handle_parameter), std::move(player_state), std::move(request));
}

celeritas::mail_sync::mail_sync(protobuf_handle_parameter_shared_ptr handle_parameter, player_state_shared_ptr player_state, request_type request)
    : base_type{ std::move(handle_parameter), std::move(player_state) },
      request_{ std::move(request) },
      player_mail_component_{ get_player_state()->get_component<player_mail_component>() }
{
}

celeritas::player_service_base::void_awaitable_type celeritas::mail_sync::response()
{
    co_await player_mail_component_->sync_mail(get_rpc(), request_.max_mail_id(), underlying_cast_enum<language_type>(request_.language_type()));
}
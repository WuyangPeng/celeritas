#include "mail_read.h"
#include "player_mail_component.h"
#include "common/logging/logger.h"
#include "initializer/initializer_constant.h"
#include "message/basic/header.h"
#include "proto/celeritas.pb.h"

celeritas::mail_read::mail_read_shared_ptr celeritas::mail_read::create(protobuf_handle_parameter_shared_ptr handle_parameter, player_state_shared_ptr player_state, request_type request)
{
    return std::make_shared<class_type>(std::move(handle_parameter), std::move(player_state), std::move(request));
}

celeritas::mail_read::mail_read(protobuf_handle_parameter_shared_ptr handle_parameter, player_state_shared_ptr player_state, request_type request)
    : base_type{ std::move(handle_parameter), std::move(player_state) },
      request_{ std::move(request) },
      player_mail_component_{ get_player_state()->get_component<player_mail_component>() }
{
}

celeritas::player_service_base::void_awaitable_type celeritas::mail_read::response()
{
    const auto error = player_mail_component_->read_mail(request_.mail_id());

    if (error != game_error_type::success)
    {
        get_player_state()->send_error_message(get_rpc(), error);
        co_return;
    }

    const header header{ get_rpc(), get_player_state()->get_user_id() };
    proto::celeritas response{};
    response.mutable_celeritas_response()->mutable_client()->mutable_player()->mutable_mail()->mutable_mail_read()->set_mail_id(request_.mail_id());

    if (!get_player_state()->write(gateway_type.data(), get_player_state()->get_instance_id(), header, response))
    {
        LOG_CHANNEL(player_channel, error) << "send message error.";
    }
}
#include "mail_delete_all_read.h"
#include "player_mail_component.h"
#include "common/logging/logger.h"
#include "initializer/initializer_constant.h"
#include "message/basic/header.h"
#include "proto/celeritas.pb.h"

celeritas::mail_delete_all_read::mail_delete_all_read_shared_ptr celeritas::mail_delete_all_read::create(protobuf_handle_parameter_shared_ptr handle_parameter, player_state_shared_ptr player_state, request_type request)
{
    return std::make_shared<class_type>(std::move(handle_parameter), std::move(player_state), std::move(request));
}

celeritas::mail_delete_all_read::mail_delete_all_read(protobuf_handle_parameter_shared_ptr handle_parameter, player_state_shared_ptr player_state, request_type request)
    : base_type{ std::move(handle_parameter), std::move(player_state) },
      request_{ std::move(request) },
      player_mail_component_{ get_player_state()->get_component<player_mail_component>() }
{
}

celeritas::player_service_base::void_awaitable_type celeritas::mail_delete_all_read::response()
{
    const auto mail_ids = co_await player_mail_component_->delete_all_read_mails();

    if (mail_ids.empty())
    {
        get_player_state()->send_error_message(get_rpc(), game_error_type::mail_not_read);
        co_return;
    }

    const header header{ get_rpc(), get_player_state()->get_user_id() };
    proto::celeritas response{};
    auto* mail_delete_all_read_response = response.mutable_celeritas_response()->mutable_client()->mutable_player()->mutable_mail()->mutable_mail_delete_all_read();

    for (const auto mail_id : mail_ids)
    {
        mail_delete_all_read_response->add_mail_ids(mail_id);
    }

    if (!get_player_state()->write(gateway_type.data(), get_player_state()->get_instance_id(), header, response))
    {
        LOG_CHANNEL(player_channel, error) << "send message error.";
    }
}
#include "add_item.h"
#include "debug_base.h"
#include "common/core/celeritas_error.h"
#include "common/core/enum_cast.h"
#include "common/logging/logger.h"
#include "initializer/initializer_constant.h"
#include "proto/celeritas.pb.h"

celeritas::debug_base::debug_shared_ptr celeritas::debug_base::create(protobuf_handle_parameter_shared_ptr handle_parameter, player_state_shared_ptr player_state, request_type request)
{
    switch (const auto type = request.type())
    {
        case proto::client::add_item:
        {
            return std::make_shared<add_item>(std::move(handle_parameter), std::move(player_state), std::move(request));
        }

        default:
        {
            throw celeritas_error{ "debug type is no exist,type = {}", enum_cast_underlying(type) };
        }
    }
}

celeritas::debug_base::debug_base(protobuf_handle_parameter_shared_ptr handle_parameter, player_state_shared_ptr player_state, request_type request)
    : base_type{ std::move(handle_parameter), std::move(player_state) }, request_{ std::move(request) }
{
}

celeritas::player_service_base::void_awaitable_type celeritas::debug_base::response()
{
    if (const auto game_error = co_await do_response();
        game_error == game_error_type::success)
    {
        const header header{ get_rpc(), get_player_state()->get_user_id() };

        proto::celeritas response{};
        response.mutable_celeritas_response()->mutable_client()->mutable_player()->mutable_debug()->mutable_debug();
        if (!get_player_state()->write(gateway_type.data(), get_player_state()->get_instance_id(), header, response))
        {
            LOG_CHANNEL(player_channel, error) << "send message error.";
        }
    }
    else
    {
        get_player_state()->send_error_message(get_rpc(), game_error);
    }
}

int64_t celeritas::debug_base::get_id() const
{
    return request_.id();
}

int64_t celeritas::debug_base::get_parameter() const
{
    return request_.parameter();
}
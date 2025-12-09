#include "gateway_login.h"
#include "database/database_pool_manager.h"
#include "database/generated/redis/auth/character_session.h"
#include "database/generated/redis/auth/session_token.h"
#include "initializer/initializer_fwd.h"
#include "message/game_error_type.h"
#include "proto/celeritas.pb.h"
#include "service_registry/service_info.h"
#include "service_registry/service_registry.h"

celeritas::gateway_login::gateway_login(protobuf_handle_parameter protobuf_handle_parameter, const proto::client::login_request& login)
    : protobuf_handle_parameter_{ std::move(protobuf_handle_parameter) }, login_{ login }
{
}

void celeritas::gateway_login::send_error_message(game_error_type game_error_type) const
{
    proto::celeritas response{};
    response.mutable_celeritas_response()->mutable_client()->mutable_player()->mutable_login()->mutable_login();

    protobuf_handle_parameter_.write(header{ protobuf_handle_parameter_.get_rpc(), static_cast<int>(game_error_type) }, response);
}

void celeritas::gateway_login::write_to_server(const session_token& session_token, const std::string& instance_id) const
{
    proto::celeritas request{};
    auto* gateway_login = request.mutable_celeritas_request()->mutable_service()->mutable_player()->mutable_gateway_login();
    gateway_login->set_account_id(session_token.get_account_id());
    gateway_login->set_game_server_id(login_.game_server_id());
    gateway_login->set_device_id(login_.device_id());
    gateway_login->set_app_version(login_.app_version());

    protobuf_handle_parameter_.write_to_server(player_type.data(), instance_id, request);
}

celeritas::gateway_login::void_awaitable_type celeritas::gateway_login::send_message() const
{
    const auto redis_pool = database_pool_manager::get_instance().get_pool(redis_db_name.data());

    const auto optional_session_token = co_await redis_pool->select_one(session_token::get_select(database_type::redis, login_.token()), session_token::get_database_field_container());
    if (!optional_session_token)
    {
        co_return send_error_message(game_error_type::token_error);
    }

    const session_token session_token{ *optional_session_token };

    if (const auto optional_character_session = co_await redis_pool->select_one(character_session::get_select(database_type::redis, session_token.get_account_id()), character_session::get_database_field_container()))
    {
        character_session character_session{ *optional_character_session };

        if (const auto optional_services_info = service_registry::get_services_by_instance_id(character_session.get_player_server_instance_id()))
        {
            co_return write_to_server(session_token, character_session.get_player_server_instance_id());
        }
    }

    const auto optional_services_info = service_registry::get_idle_services(player_type.data(), login_.game_server_id());
    if (!optional_services_info)
    {
        co_return send_error_message(game_error_type::server_error);
    }

    co_return write_to_server(session_token, optional_services_info->get_instance_id());
}
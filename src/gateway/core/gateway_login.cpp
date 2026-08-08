#include "gateway_login.h"
#include "common/logging/logger.h"
#include "database/database_constant.h"
#include "database/pool/database_pool_manager.h"
#include "database/generated/redis/auth/character_session.h"
#include "database/generated/redis/auth/session_token.h"
#include "initializer/initializer_constant.h"
#include "message/basic/game_error_type.h"
#include "proto/celeritas.pb.h"
#include "service_registry/core/service_registry.h"
#include "service_registry/data/service_info.h"

celeritas::gateway_login::gateway_login(protobuf_handle_parameter_shared_ptr protobuf_handle_parameter, const proto::client::login_request& login)
    : protobuf_handle_parameter_{ std::move(protobuf_handle_parameter) }, login_{ login }
{
}

void celeritas::gateway_login::send_error_message(game_error_type game_error_type) const
{
    proto::celeritas response{};
    response.mutable_celeritas_response()->mutable_client()->mutable_player()->mutable_login()->mutable_login();

    protobuf_handle_parameter_->write_to_response(header{ protobuf_handle_parameter_->get_rpc(), game_error_type }, response);
}

void celeritas::gateway_login::write_to_server(const session_token& session_token, const std::string& instance_id, const bool new_game_server_id) const
{
    proto::celeritas request{};
    auto* service_login = request.mutable_celeritas_request()->mutable_service()->mutable_player()->mutable_service_login();
    service_login->set_account_id(session_token.get_account_id());
    service_login->set_new_account(session_token.is_new_account());
    service_login->set_account_bind_id(session_token.get_account_bind_id());
    service_login->set_game_server_id(login_.game_server_id());
    service_login->set_device_id(login_.device_id());
    service_login->set_app_version(login_.app_version());
    service_login->set_new_game_server_id(new_game_server_id);
    service_login->set_session_id(protobuf_handle_parameter_->get_session_id());
    service_login->set_protocol(static_cast<int>(protobuf_handle_parameter_->get_server_network_type()));
    service_login->set_ip_address(protobuf_handle_parameter_->get_remote_ip_address());

    protobuf_handle_parameter_->write_to_server(player_type.data(), instance_id, request);
}

celeritas::gateway_login::void_awaitable_type celeritas::gateway_login::response() const
{
    LOG_CHANNEL(gateway_channel, debug) << "gateway login begin. token = " << login_.token();

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
            co_return write_to_server(session_token, character_session.get_player_server_instance_id(), false);
        }
    }

    const auto optional_services_info = service_registry::get_idle_services(player_type.data(), login_.game_server_id());
    if (!optional_services_info)
    {
        co_return send_error_message(game_error_type::server_error);
    }

    LOG_CHANNEL(gateway_channel, debug) << "gateway login to player server. token = " << login_.token();

    co_return write_to_server(session_token, optional_services_info->get_instance_id(), true);
}

 
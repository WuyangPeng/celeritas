#include "create_account.h"
#include "create_user.h"
#include "service_login.h"
#include "common/core/noexcept_safe_call_and_log.h"
#include "common/logging/logger.h"
#include "common/core/time_helper.h"
#include "config/aggregate/app_config.h"
#include "database/database_constant.h"
#include "database/database_fwd.h"
#include "database/pool/database_pool_manager.h"
#include "database/generated/mysql/player/user.h"
#include "message/basic/game_error_type.h"
#include "message/parameters/protobuf_handle_parameter.h"
#include "player/component/player_manager.h"
#include "player/component/player_state.h"
#include "player/component/player_state_type.h"
#include "proto/celeritas.pb.h"

celeritas::service_login::service_login(protobuf_handle_parameter_shared_ptr protobuf_handle_parameter, const proto::service::service_login_request& login)
    : protobuf_handle_parameter_{ std::move(protobuf_handle_parameter) }, login_{ login }, is_new_user_{ false }
{
}

celeritas::service_login::int64_awaitable_type celeritas::service_login::response()
{
    if (login_.new_account())
    {
        LOG_CHANNEL(player_channel, debug) << "login new account,account = " << login_.account_id() << ",bind id = " << login_.account_bind_id();

        if (const create_account create_account{ protobuf_handle_parameter_, login_ };
            !co_await create_account.save_database())
        {
            send_error_message(game_error_type::create_account_error);
            co_return 0;
        }
    }

    const auto user = co_await get_user();
    if (!user)
    {
        co_return 0;
    }

    LOG_CHANNEL(player_channel, debug) << "login add player = " << login_.account_id() << ",bind id = " << login_.account_bind_id();

    const auto player = player_manager::get_instance().add_player(*user, protobuf_handle_parameter_->get_resource_loader(), protobuf_handle_parameter_->get_any_io_executor(), protobuf_handle_parameter_->get_instance_id(), login_, is_new_user_);

    send_success_message(user->get_user_id());

    co_spawn(player->get_any_io_executor(),
             noexcept_safe_call_and_log_awaitable([player = player,
                                                      user = user,
                                                      self = shared_from_this()] {
                                                      return self->load_player(user, player);;
                                                  },
                                                  player_channel,
                                                  "load player error: "),
             boost::asio::detached);

    co_return user->get_user_id();
}

void celeritas::service_login::send_error_message(game_error_type game_error_type) const
{
    const header header{ protobuf_handle_parameter_->get_rpc(), game_error_type };

    proto::celeritas response{};
    response.mutable_celeritas_response()->mutable_service()->mutable_player()->mutable_service_login();

    protobuf_handle_parameter_->write_to_response(header, response);
}

void celeritas::service_login::send_success_message(const int64_t user_id) const
{
    const header header{ protobuf_handle_parameter_->get_rpc(), user_id, game_error_type::success };

    proto::celeritas response{};
    auto* login = response.mutable_celeritas_response()->mutable_service()->mutable_player()->mutable_service_login();
    login->set_current_time(time_helper::get_current_milliseconds());
    login->set_session_id(login_.session_id());
    login->set_protocol(login_.protocol());
    login->set_instance_id(protobuf_handle_parameter_->get_app_config()->get_server_config()->get_instance_id());

    protobuf_handle_parameter_->write_to_response(header, response);
}

celeritas::service_login::optional_user_awaitable_type celeritas::service_login::get_user()
{
    const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_player_db_name.data());
    const auto key = std::make_shared<basis_database_container>(basis_database_container::object_container{ { user::account_id_describe, login_.account_id() },
                                                                                                            { user::game_server_id_describe, login_.game_server_id() } });

    const auto optional_user = co_await mysql_pool->select_one(user::get_select(database_type::mysql, key), user::get_database_field_container());
    if (!optional_user)
    {
        const create_user create_user{ protobuf_handle_parameter_, login_ };
        auto user = co_await create_user.save_database();
        if (!user)
        {
            send_error_message(game_error_type::create_user_error);

            co_return std::nullopt;
        }

        is_new_user_ = true;
        co_return user;
    }

    co_return user{ *optional_user };
}

celeritas::service_login::void_awaitable_type celeritas::service_login::load_player(const optional_user& user, const player_state_shared_ptr& player) const
{
    if (player->get_player_state_type() != player_state_type::online || user->is_overload_db() || login_.new_game_server_id())
    {
        co_await player->on_load_db();
        co_await player->on_db_analysis(protobuf_handle_parameter_->get_app_config());
        co_await player->on_register_event();

        player->set_player_state_type(player_state_type::online);
    }

    co_await player->on_dependencies_ready();
    co_await player->send_initial_sync();
    co_await player->on_login();

    player->set_dirty();

    co_return;
}
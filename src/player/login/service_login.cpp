#include "create_account.h"
#include "create_user.h"
#include "service_login.h"
#include "common/time_helper.h"
#include "config/app_config.h"
#include "database/database_fwd.h"
#include "database/database_pool_manager.h"
#include "database/generated/mysql/player/user.h"
#include "message/game_error_type.h"
#include "player/component/player_manager.h"
#include "player/component/player_state.h"
#include "player/component/player_state_type.h"
#include "proto/celeritas.pb.h"

celeritas::service_login::service_login(protobuf_handle_parameter protobuf_handle_parameter, const proto::service::service_login_request& login)
    : protobuf_handle_parameter_{ std::move(protobuf_handle_parameter) }, login_{ login }
{
}

celeritas::service_login::void_awaitable_type celeritas::service_login::send_message() const
{
    if (login_.new_account())
    {
        if (const create_account create_account{ protobuf_handle_parameter_, login_ };
            !co_await create_account.send_message())
        {
            co_return send_error_message(game_error_type::create_account_error);
        }
    }

    auto user = co_await get_user();
    if (!user)
    {
        co_return;
    }

    const auto player = player_manager::get_instance().add_player(*user, protobuf_handle_parameter_.get_resource_loader());

    send_success_message(user->get_user_id());

    if (player->get_player_state_type() != player_state_type::online || user->is_overload_db() || login_.new_game_server_id())
    {
        co_await player->on_load_db();
        co_await player->on_db_analysis();

        player->set_player_state_type(player_state_type::online);
    }

    co_await player->on_dependencies_ready();
    co_await player->send_initial_sync();
    co_await player->on_login();

    player->set_dirty();

    co_return;
}

void celeritas::service_login::send_error_message(game_error_type game_error_type) const
{
    const header header{ protobuf_handle_parameter_.get_rpc(), static_cast<int>(game_error_type) };

    proto::celeritas response{};
    response.mutable_celeritas_response()->mutable_service()->mutable_player()->mutable_service_login();

    protobuf_handle_parameter_.write(header, response);
}

void celeritas::service_login::send_success_message(const int64_t user_id) const
{
    const header header{ protobuf_handle_parameter_.get_rpc(), user_id, static_cast<int>(game_error_type::success) };

    proto::celeritas response{};
    auto* login = response.mutable_celeritas_response()->mutable_service()->mutable_player()->mutable_service_login();
    login->set_current_time(time_helper::get_current_milliseconds());
    login->set_session_id(login_.session_id());
    login->set_protocol(login_.protocol());
    login->set_instance_id(protobuf_handle_parameter_.get_app_config()->get_server_config().get_instance_id());

    protobuf_handle_parameter_.write(header, response);
}

celeritas::service_login::optional_user_awaitable_type celeritas::service_login::get_user() const
{
    const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_player_db_name.data());
    const auto key = std::make_shared<basis_database_container>(basis_database_container::object_container{ { user::account_id_describe, login_.account_id() },
                                                                                                            { user::game_server_id_describe, login_.game_server_id() } });

    const auto optional_user = co_await mysql_pool->select_one(user::get_select(database_type::mysql, key), user::get_database_field_container());
    if (!optional_user)
    {
        const create_user create_user{ protobuf_handle_parameter_, login_ };
        auto user = co_await create_user.send_message();
        if (!user)
        {
            send_error_message(game_error_type::create_user_error);

            co_return std::nullopt;
        }

        co_return user;
    }

    co_return user{ *optional_user };
}
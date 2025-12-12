#include "player_state.h"
#include "player_user_component.h"
#include "common/logger.h"
#include "database/database_pool_manager.h"
#include "initializer/initializer_fwd.h"
#include "message/game_error_type.h"
#include "message/header.h"
#include "proto/celeritas.pb.h"

celeritas::player_user_component::player_user_component(user user, player_state* player_state) noexcept
    : base_type{ get_player_component_type(), player_state }, user_{ std::move(user) }
{
}

celeritas::player_user_component::void_awaitable_type celeritas::player_user_component::on_db_analysis()
{
    user_.set_overload_db(false);

    co_return;
}

celeritas::player_user_component::void_awaitable_type celeritas::player_user_component::save_db()
{
    if (user_.is_modify())
    {
        const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_player_db_name.data());

        co_await mysql_pool->execute_changes(user_.get_modify());

        user_.clear_modify();
    }
}

celeritas::player_component::void_awaitable_type celeritas::player_user_component::on_login()
{
    const header header{ user_.get_user_id() };

    proto::celeritas response{};
    response.mutable_celeritas_response()->mutable_client()->mutable_player()->mutable_login()->mutable_login_finish();

    if (!get_player_state()->write(gateway_type.data(), "", header, response))
    {
        LOG_CHANNEL(message_channel, error) << "send message error.";
    }

    co_return;
}

int64_t celeritas::player_user_component::get_user_id() const noexcept
{
    return user_.get_user_id();
}

std::string celeritas::player_user_component::get_game_server_id() const
{
    return user_.get_game_server_id();
}
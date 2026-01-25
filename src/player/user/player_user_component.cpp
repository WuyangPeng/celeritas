#include "player_user_component.h"
#include "database/pool/database_pool_manager.h"
#include "message/basic/game_error_type.h"
#include "player/component/player_state.h"
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
        if (const auto mysql_pool = get_mysql_player_database_pool();
            co_await mysql_pool->execute_changes(user_.get_modify()))
        {
            user_.clear_modify();
        }
    }
}

int64_t celeritas::player_user_component::get_user_id() const noexcept
{
    return user_.get_user_id();
}

std::string celeritas::player_user_component::get_game_server_id() const
{
    return user_.get_game_server_id();
}

bool celeritas::player_user_component::is_modify() const
{
    return user_.is_modify();
}

bool celeritas::player_user_component::is_overload_db() const
{
    return user_.is_overload_db();
}

int64_t celeritas::player_user_component::get_account_id() const noexcept
{
    return user_.get_account_id();
}


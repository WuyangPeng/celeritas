#include "player_user_component.h"
#include "database/database_pool_manager.h"

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

int64_t celeritas::player_user_component::get_user_id() const noexcept
{
    return user_.get_user_id();
}

std::string celeritas::player_user_component::get_game_server_id() const
{
    return user_.get_game_server_id();
}
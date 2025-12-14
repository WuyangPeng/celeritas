#include "player_time_component.h"
#include "config/database_type.h"
#include "database/database_pool_base.h"
#include "player/component/player_state.h"

celeritas::player_time_component::player_time_component(player_state* player_state) noexcept
    : base_type{ get_player_component_type(), player_state }, user_server_roles_{}
{
}

celeritas::player_component::void_awaitable_type celeritas::player_time_component::on_load_db()
{
    const auto mongo_pool = get_mongo_player_db_name();
    const auto user_id = get_player_state()->get_user_id();

    const auto optional_user_server_roles = co_await mongo_pool->select_one(user_server_roles::get_select(database_type::mongo, user_id), user_server_roles::get_database_field_container());

    if (optional_user_server_roles)
    {
        user_server_roles_ = user_server_roles{ *optional_user_server_roles };
    }
    else
    {
        user_server_roles_ = user_server_roles{ database_type::mongo, user_id };
    }
}

celeritas::player_component::void_awaitable_type celeritas::player_time_component::save_db()
{
    if (user_server_roles_->is_must_save())
    {
        const auto mongo_pool = get_mongo_player_db_name();

        co_await mongo_pool->execute_changes(user_server_roles_->get_modify());

        user_server_roles_->clear_modify();
    }
}

bool celeritas::player_time_component::is_modify() const
{
    return user_server_roles_->is_must_save();
}
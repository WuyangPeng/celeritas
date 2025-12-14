#include "player_time_component.h"
#include "config/database_type.h"
#include "database/database_pool_base.h"
#include "player/component/player_state.h"

celeritas::player_time_component::player_time_component(player_state* player_state) noexcept
    : base_type{ get_player_component_type(), player_state }, user_time_refresh_{}, player_time_refresh_{}
{
}

celeritas::player_component::void_awaitable_type celeritas::player_time_component::on_load_db()
{
    const auto mongo_pool = get_mongo_player_db_name();
    const auto user_id = get_player_state()->get_user_id();

    if (const auto optional_user_time_refresh = co_await mongo_pool->select_one(user_time_refresh::get_select(database_type::mongo, user_id), user_time_refresh::get_database_field_container()))
    {
        user_time_refresh_ = user_time_refresh{ *optional_user_time_refresh };
    }
    else
    {
        user_time_refresh_ = user_time_refresh{ database_type::mongo, user_id };
    }

    for (const auto& element : user_time_refresh_->get_player_time_refresh())
    {
        player_time_refresh_.emplace_back(player_time_refresh::from_json_string(element));
    }
}

celeritas::player_component::void_awaitable_type celeritas::player_time_component::save_db()
{
    if (user_time_refresh_->is_must_save())
    {
        const auto mongo_pool = get_mongo_player_db_name();

        co_await mongo_pool->execute_changes(user_time_refresh_->get_modify());

        user_time_refresh_->clear_modify();
    }
}

bool celeritas::player_time_component::is_modify() const
{
    return user_time_refresh_->is_must_save();
}
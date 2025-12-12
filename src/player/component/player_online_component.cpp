#include "player_online_component.h"
#include "player_state.h"
#include "common/time_helper.h"
#include "database/database_pool_manager.h"

celeritas::player_online_component::player_online_component(player_state* player_state) noexcept
    : base_type{ get_player_component_type(), player_state }, account_last_login_{}
{
}

celeritas::player_component::void_awaitable_type celeritas::player_online_component::on_load_db()
{
    const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_auth_db_name.data());

    const auto* player_state = get_player_state();

    if (const auto optional_account_last_login = co_await mysql_pool->select_one(account_last_login::get_select(database_type::mysql, player_state->get_user_id()), account_last_login::get_database_field_container()))
    {
        account_last_login_ = account_last_login{ *optional_account_last_login };
    }
    else
    {
        account_last_login_ = account_last_login{ database_type::mysql, player_state->get_user_id() };
    }

    account_last_login_->set_game_server_id(player_state->get_game_server_id());
    account_last_login_->set_update_time(time_helper::get_current_milliseconds());
}

celeritas::player_component::void_awaitable_type celeritas::player_online_component::save_db()
{
    if (account_last_login_->is_modify())
    {
        const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_auth_db_name.data());

        co_await mysql_pool->execute_changes(account_last_login_->get_modify());

        account_last_login_->clear_modify();
    }
}

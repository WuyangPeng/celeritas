#include "player_red_dot_database.h"
#include "common/core/time_helper.h"
#include "config/basic/database_type.h"
#include "database/pool/database_pool_base.h"
#include "player/component/player_state.h"
#include "player/red_dot/player_red_dot_component.h"
#include "player/user/player_user_component.h"

celeritas::player_red_dot_database::player_red_dot_database(player_state* player_state, player_red_dot_component* player_red_dot_component)
    : player_state_{ player_state }, player_red_dot_component_{ player_red_dot_component }
{
}

celeritas::player_red_dot_database::void_awaitable_type celeritas::player_red_dot_database::load_user_red_dots()
{
    const auto mongo_player_pool = player_red_dot_component_->get_mongo_player_database_pool();
    const auto player_user = player_state_->get_component<player_user_component>();
    const auto user_id = player_user->get_user_id();

    if (const auto database_entity_change = co_await mongo_player_pool->select_one(user_red_dots::get_select(database_type::mongo, user_id), user_red_dots::get_database_field_container()))
    {
        user_red_dots_ = user_red_dots{ *database_entity_change };
    }
    else
    {
        user_red_dots_ = user_red_dots{ database_type::mongo, user_id };
    }
}

celeritas::traits::document_array_type celeritas::player_red_dot_database::get_red_dots() const
{
    return user_red_dots_->get_red_dots();
}

void celeritas::player_red_dot_database::set_red_dots(traits::param_type::document_array_type red_dots)
{
    user_red_dots_->set_red_dots(red_dots);
    user_red_dots_->set_last_check_time(time_helper::get_current_milliseconds());

    player_state_->set_dirty();
}

celeritas::player_red_dot_database::void_awaitable_type celeritas::player_red_dot_database::save_db()
{
    if (user_red_dots_->is_must_save())
    {
        const auto mongo_player_pool = player_red_dot_component_->get_mongo_player_database_pool();
        if (co_await mongo_player_pool->execute_changes(user_red_dots_->get_modify()))
        {
            user_red_dots_->clear_modify();
        }
    }
}

bool celeritas::player_red_dot_database::is_modify() const
{
    return user_red_dots_->is_must_save();
}
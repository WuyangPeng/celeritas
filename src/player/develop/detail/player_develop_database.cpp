#include "player_develop_database.h"
#include "player_develop_internal_fwd.h"
#include "common/core/time_helper.h"
#include "config/database_type.h"
#include "database/database_pool_base.h"
#include "player/component/player_state.h"
#include "player/develop/player_develop_component.h"
#include "player/user/player_user_component.h"

celeritas::player_develop_database::player_develop_database(player_state* player_state, player_develop_component* player_develop_component)
    : player_state_{ player_state }, player_develop_component_{ player_develop_component }, user_develop_{}
{
}

celeritas::player_develop_database::void_awaitable_type celeritas::player_develop_database::load_user_develop()
{
    const auto mongo_player_pool = player_develop_component_->get_mongo_player_database_pool();
    const auto player_user = player_state_->get_component<player_user_component>();
    const auto user_id = player_user->get_user_id();

    if (const auto database_entity_change = co_await mongo_player_pool->select_one(user_develop::get_select(database_type::mongo, user_id), user_develop::get_database_field_container()))
    {
        user_develop_ = user_develop{ *database_entity_change };
    }
    else
    {
        user_develop_ = user_develop{ database_type::mongo, user_id };
    }
}

celeritas::traits::document_array_type celeritas::player_develop_database::get_develop_data() const
{
    return user_develop_->get_develop_data();
}

void celeritas::player_develop_database::set_develop(traits::param_type::document_array_type develop)
{
    user_develop_->set_develop_data(develop);
    user_develop_->set_data_version(develop_data_version);
    user_develop_->set_last_save_time(time_helper::get_current_milliseconds());

    player_state_->set_dirty();
}

celeritas::player_develop_database::void_awaitable_type celeritas::player_develop_database::save_db()
{
    if (user_develop_->is_must_save())
    {
        const auto mongo_player_pool = player_develop_component_->get_mongo_player_database_pool();
        if (co_await mongo_player_pool->execute_changes(user_develop_->get_modify()))
        {
            user_develop_->clear_modify();
        }
    }
}

bool celeritas::player_develop_database::is_modify() const
{
    return user_develop_->is_must_save();
}
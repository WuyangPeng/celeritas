#include "player_item_database.h"
#include "player_item_internal_fwd.h"
#include "common/core/time_helper.h"
#include "config/basic/database_type.h"
#include "database/pool/database_pool_base.h"
#include "player/component/player_state.h"
#include "player/item/player_item_component.h"
#include "player/user/player_user_component.h"

celeritas::player_item_database::player_item_database(player_state* player_state, player_item_component* player_item_component)
    : player_state_{ player_state }, player_item_component_{ player_item_component }, user_item_{}
{
}

celeritas::player_item_database::void_awaitable_type celeritas::player_item_database::load_user_item()
{
    const auto mongo_player_pool = player_item_component_->get_mongo_player_database_pool();
    const auto player_user = player_state_->get_component<player_user_component>();
    const auto user_id = player_user->get_user_id();

    if (const auto database_entity_change = co_await mongo_player_pool->select_one(user_item::get_select(database_type::mongo, user_id), user_item::get_database_field_container()))
    {
        user_item_ = user_item{ *database_entity_change };
    }
    else
    {
        user_item_ = user_item{ database_type::mongo, user_id };
    }
}

celeritas::traits::document_array_type celeritas::player_item_database::get_inventory_data() const
{
    return user_item_->get_inventory_data();
}

void celeritas::player_item_database::set_item(traits::param_type::document_array_type item)
{
    user_item_->set_inventory_data(item);
    user_item_->set_data_version(item_data_version);
    user_item_->set_last_save_time(time_helper::get_current_milliseconds());

    player_state_->set_dirty();
}

celeritas::player_item_database::void_awaitable_type celeritas::player_item_database::save_db()
{
    if (user_item_->is_must_save())
    {
        const auto mongo_player_pool = player_item_component_->get_mongo_player_database_pool();
        if (co_await mongo_player_pool->execute_changes(user_item_->get_modify()))
        {
            user_item_->clear_modify();
        }
    }
}

bool celeritas::player_item_database::is_modify() const
{
    return user_item_->is_must_save();
}
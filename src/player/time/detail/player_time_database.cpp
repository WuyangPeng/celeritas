#include "player_time_database.h"
#include "config/database_type.h"
#include "database/database_pool_base.h"
#include "player/component/player_state.h"
#include "player/time/player_time_component.h"

celeritas::player_time_database::player_time_database(player_state* player_state, player_time_component* player_time_component)
    : player_state_{ player_state }, player_time_component_{ player_time_component }, user_time_refresh_{}
{
}

celeritas::player_time_database::player_time_refresh_container_awaitable_type celeritas::player_time_database::load()
{
    player_time_refresh_container container{};
    const auto mongo_pool = player_time_component_->get_mongo_player_db_name();
    const auto user_id = player_state_->get_user_id();

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
        auto player_time = player_time_refresh::from_json_string(element);
        container.emplace(player_time_refresh_key{ player_time.get_time_refresh_type(), player_time.get_parameter() }, std::move(player_time));
    }

    co_return container;
}

void celeritas::player_time_database::update_document(const player_time_refresh_container& container)
{
    traits::document_array_type documents{};
    for (auto& element : container | std::views::values)
    {
        documents.emplace_back(element.to_json_string());
    }

    user_time_refresh_->set_player_time_refresh(documents);
}

celeritas::player_time_database::void_awaitable_type celeritas::player_time_database::save()
{
    if (user_time_refresh_->is_must_save())
    {
        const auto mongo_pool = player_time_component_->get_mongo_player_db_name();
        co_await mongo_pool->execute_changes(user_time_refresh_->get_modify());
        user_time_refresh_->clear_modify();
    }
}

bool celeritas::player_time_database::is_must_save() const
{
    return user_time_refresh_->is_must_save();
}

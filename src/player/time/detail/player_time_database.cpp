#include "player_time_database.h"
#include "player_time_document.h"
#include "config/basic/database_type.h"
#include "database/database_pool_base.h"
#include "player/component/player_state.h"
#include "player/time/player_time_component.h"
#include "player/time/player_time_refresh_key.h"

celeritas::player_time_database::player_time_database(const int64_t user_id, player_time_component* player_time_component, player_time_document* player_time_document)
    : user_id_{ user_id }, player_time_component_{ player_time_component }, player_time_document_{ player_time_document }, user_time_refresh_{}
{
}

celeritas::player_time_database::player_time_refresh_container_awaitable_type celeritas::player_time_database::load()
{
    co_await load_user_time_refresh();

    co_return load_player_time_refresh();
}

void celeritas::player_time_database::update_document()
{
    const auto& container = player_time_document_->get_player_time_refresh_container();
    traits::document_array_type documents{};
    for (auto& element : container | std::views::values)
    {
        documents.emplace_back(element.to_document_type());
    }

    user_time_refresh_->set_player_time_refresh(documents);
}

celeritas::player_time_database::void_awaitable_type celeritas::player_time_database::save()
{
    if (user_time_refresh_->is_must_save())
    {
        const auto mongo_pool = player_time_component_->get_mongo_player_database_pool();
        if (co_await mongo_pool->execute_changes(user_time_refresh_->get_modify()))
        {
            user_time_refresh_->clear_modify();
        }
    }
}

bool celeritas::player_time_database::is_must_save() const
{
    return user_time_refresh_->is_must_save();
}

celeritas::player_time_database::void_awaitable_type celeritas::player_time_database::load_user_time_refresh()
{
    const auto mongo_pool = player_time_component_->get_mongo_player_database_pool();

    if (const auto optional_user_time_refresh = co_await mongo_pool->select_one(user_time_refresh::get_select(database_type::mongo, user_id_), user_time_refresh::get_database_field_container()))
    {
        user_time_refresh_ = user_time_refresh{ *optional_user_time_refresh };
    }
    else
    {
        user_time_refresh_ = user_time_refresh{ database_type::mongo, user_id_ };
    }
}

celeritas::player_time_database::player_time_refresh_container celeritas::player_time_database::load_player_time_refresh()
{
    player_time_refresh_container container{};
    for (const auto& element : user_time_refresh_->get_player_time_refresh())
    {
        auto player_time = player_time_refresh::from_document(element);
        player_time_refresh_key player_time_refresh_key{ player_time };
        container.emplace(player_time_refresh_key, std::move(player_time));
    }

    return container;
}

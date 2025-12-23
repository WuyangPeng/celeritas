#include "player_develop_document.h"
#include "config/game_config/develop_config.h"
#include "config/game_config/game_config.h"
#include "config/game_config/game_tables.h"
#include "database/config/config_manager.h"
#include "message/celeritas_game_error.h"

#include <ranges>

void celeritas::player_develop_document::set_develop(traits::param_type::document_array_type develop_document)
{
    for (const auto& element : develop_document)
    {
        auto develop_data = develop_data::from_json_string(element);
        add_develop_data(develop_data);
    }
}

celeritas::traits::document_array_type celeritas::player_develop_document::get_develop() const
{
    traits::document_array_type documents{};
    for (auto& element : develop_data_ | std::views::values)
    {
        documents.emplace_back(element.to_json_string());
    }

    return documents;
}

celeritas::game_error_type celeritas::player_develop_document::develop_level(const develop_data_key& key)
{
    const auto develop_config = game_config::get_instance().get_game_tables()->get_develop_config()->get(key.get_system_id());
    if (!develop_config)
    {
        throw celeritas_error{ "develop config not found,id = {}", key.get_system_id() };
    }

    if (const auto iter = develop_data_.find(key); iter != develop_data_.cend())
    {
        if (iter->second.get_level() == (*develop_config)->get_max_level())
        {
            return game_error_type::max_develop;
        }

        iter->second.add_level();
    }
    else
    {
        develop_data_.emplace(key, develop_data{ key.get_system_id(), key.get_instance_id() });
    }

    return game_error_type::success;
}

celeritas::game_error_type celeritas::player_develop_document::develop_reset(develop_data_key key)
{
    const auto develop_config = game_config::get_instance().get_game_tables()->get_develop_config()->get(key.get_system_id());
    if (!develop_config)
    {
        throw celeritas_error{ "develop config not found,id = {}", key.get_system_id() };
    }

    if ((*develop_config)->get_develop_reset_type() == develop_reset_type::non_resettable)
    {
        return game_error_type::non_resettable;
    }

    if (const auto iter = develop_data_.find(key); iter != develop_data_.cend())
    {
        if (iter->second.get_level() == 0)
        {
            return game_error_type::non_resettable;
        }

        iter->second.clear();
    }
    else
    {
        return game_error_type::develop_not_exist;
    }

    return game_error_type::success;
}

void celeritas::player_develop_document::add_develop_data(const develop_data& develop_data)
{
    develop_data_.emplace(develop_data_key{ develop_data.get_system_id(), develop_data.get_instance_id() }, develop_data);
}
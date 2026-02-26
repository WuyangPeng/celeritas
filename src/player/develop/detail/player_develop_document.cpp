#include "player_develop_document.h"
#include "common/logging/logger.h"
#include "config/game/game_config.h"
#include "config/game/game_tables.h"
#include "database/config/config_manager.h"
#include "initializer/initializer_constant.h"
#include "message/basic/celeritas_game_error.h"
#include "proto/celeritas.pb.h"

#include <ranges>

celeritas::player_develop_document::player_develop_document(player_state* player_state)
    : develop_data_{}, player_state_{ player_state }
{
}

void celeritas::player_develop_document::set_develop(traits::param_type::document_array_type develop_document)
{
    for (const auto& element : develop_document)
    {
        auto develop_data = develop_data::from_document(element);
        add_develop_data(develop_data);
    }
}

celeritas::traits::document_array_type celeritas::player_develop_document::get_develop() const
{
    traits::document_array_type documents{};
    for (auto& element : develop_data_ | std::views::values)
    {
        documents.emplace_back(element.to_document_type());
    }

    return documents;
}

celeritas::game_error_type celeritas::player_develop_document::develop_level(const develop_data& develop_data)
{
    const auto develop_data_key = develop_data.get_develop_data_key();
    if (const auto iter = develop_data_.find(develop_data_key);
        iter != develop_data_.cend())
    {
        iter->second.set_level(develop_data.get_level());
    }
    else
    {
        develop_data_.emplace(develop_data_key, develop_data);
    }

    return game_error_type::success;
}

celeritas::player_develop_document::optional_develop_data celeritas::player_develop_document::develop_level(const const_develop_config_shared_ptr& develop_config)
{
    if (const auto iter = develop_data_.find(develop_data_key{ develop_config->id, 0 });
        iter != develop_data_.cend())
    {
        return iter->second;
    }

    return std::nullopt;
}

celeritas::game_error_type celeritas::player_develop_document::develop_reset(const develop_data_key& key)
{
    if (const auto iter = develop_data_.find(key);
        iter != develop_data_.cend())
    {
        iter->second.clear();
    }
    else
    {
        return game_error_type::develop_not_exist;
    }

    return game_error_type::success;
}

celeritas::game_error_type celeritas::player_develop_document::develop_claim_reward(const develop_data& develop_data)
{
    const auto develop_data_key = develop_data.get_develop_data_key();
    if (const auto iter = develop_data_.find(develop_data_key);
        iter != develop_data_.cend())
    {
        iter->second.set_reward_level(develop_data.get_reward_level());
    }
    else
    {
        return game_error_type::develop_not_exist;
    }

    return game_error_type::success;
}

void celeritas::player_develop_document::send_initial_sync()
{
    const header header{ player_state_->get_user_id() };

    proto::celeritas response{};
    auto* develop_response = response.mutable_celeritas_response()->mutable_client()->mutable_player()->mutable_develop()->mutable_develop();
    for (const auto& element : develop_data_ | std::views::values)
    {
        auto* develop = develop_response->add_develop();
        develop->set_system_id(element.get_system_id());
        develop->set_instance_id(element.get_instance_id());
        develop->set_level(element.get_level());
        develop->set_exp(element.get_exp());
        develop->set_reward_level(element.get_reward_level());
    }

    develop_response->set_is_login(true);

    if (!player_state_->write(gateway_type.data(), player_state_->get_instance_id(), header, response))
    {
        LOG_CHANNEL(player_channel, error) << "send message error.";
    }
}

celeritas::player_develop_document::optional_develop_data celeritas::player_develop_document::get_develop_data(const develop_data_key& key) const
{
    if (const auto iter = develop_data_.find(key);
        iter != develop_data_.cend())
    {
        return iter->second;
    }

    return std::nullopt;
}

void celeritas::player_develop_document::add_develop_data(const develop_data& develop_data)
{
    develop_data_.emplace(develop_data_key{ develop_data.get_system_id(), develop_data.get_instance_id() }, develop_data);
}
#include "player_develop_component.h"
#include "common/logging/logger.h"
#include "initializer/initializer_constant.h"
#include "message/basic/game_error_type.h"
#include "proto/celeritas.pb.h"

celeritas::player_develop_component::player_develop_component(player_state* player_state) noexcept
    : base_type{ get_player_component_type(), player_state },
      database_{ player_state, this },
      document_{ player_state }
{
}

celeritas::player_component::void_awaitable_type celeritas::player_develop_component::on_load_db()
{
    co_await database_.load_user_develop();

    document_.set_develop(database_.get_develop_data());

    co_return;
}

celeritas::player_component::void_awaitable_type celeritas::player_develop_component::save_db()
{
    co_return co_await database_.save_db();
}

bool celeritas::player_develop_component::is_modify() const
{
    return database_.is_modify();
}

celeritas::game_error_type celeritas::player_develop_component::develop_level(const develop_data_key& key)
{
    const auto game_error = document_.develop_level(key);
    if (game_error == game_error_type::success)
    {
        update_document();
    }
    return game_error;
}

celeritas::game_error_type celeritas::player_develop_component::develop_reset(const develop_data_key& key)
{
    const auto game_error = document_.develop_reset(key);
    if (game_error == game_error_type::success)
    {
        update_document();
    }
    return game_error;
}

celeritas::player_component::void_awaitable_type celeritas::player_develop_component::send_initial_sync()
{
    document_.send_initial_sync();

    co_return;
}

void celeritas::player_develop_component::send_level_message(int rpc, const develop_data& develop_data)
{
    const header header{ rpc, get_player_state()->get_user_id() };

    proto::celeritas response{};
    auto* develop_response = response.mutable_celeritas_response()->mutable_client()->mutable_player()->mutable_develop()->mutable_develop_level();
    auto* develop = develop_response->mutable_develop();

    develop->set_system_id(develop_data.get_system_id());
    develop->set_instance_id(develop_data.get_instance_id());
    develop->set_level(develop_data.get_level());
    develop->set_exp(develop_data.get_exp());

    if (!get_player_state()->write(gateway_type.data(), get_player_state()->get_instance_id(), header, response))
    {
        LOG_CHANNEL(player_channel, error) << "send message error.";
    }
}

void celeritas::player_develop_component::send_reset_message(int rpc, const develop_data& develop_data)
{
    const header header{ rpc, get_player_state()->get_user_id() };

    proto::celeritas response{};
    auto* develop_response = response.mutable_celeritas_response()->mutable_client()->mutable_player()->mutable_develop()->mutable_develop_reset();
    auto* develop = develop_response->mutable_develop();

    develop->set_system_id(develop_data.get_system_id());
    develop->set_instance_id(develop_data.get_instance_id());
    develop->set_level(develop_data.get_level());
    develop->set_exp(develop_data.get_exp());

    if (!get_player_state()->write(gateway_type.data(), get_player_state()->get_instance_id(), header, response))
    {
        LOG_CHANNEL(player_channel, error) << "send message error.";
    }
}

celeritas::player_develop_component::optional_develop_data celeritas::player_develop_component::get_develop_data(const develop_data_key& key) const
{
    return document_.get_develop_data(key);
}

void celeritas::player_develop_component::update_document()
{
    database_.set_develop(document_.get_develop());
}
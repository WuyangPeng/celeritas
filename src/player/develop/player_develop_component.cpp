#include "player_develop_component.h"
#include "message/game_error_type.h"

celeritas::player_develop_component::player_develop_component(player_state* player_state) noexcept
    : base_type{ get_player_component_type(), player_state },
      database_{ player_state, this },
      document_{}
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

void celeritas::player_develop_component::update_document()
{
    database_.set_develop(document_.get_develop());
}
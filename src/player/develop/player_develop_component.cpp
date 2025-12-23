#include "player_develop_component.h"

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
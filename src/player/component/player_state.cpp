#include "player_online_component.h"
#include "player_role_component.h"
#include "player_state.h"
#include "player_state_type.h"
#include "common/celeritas_error.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

celeritas::player_state::player_state(const int64_t user_id, std::string game_server_id, const resource_loader_shared_ptr& resource_loader)
    : user_id_{ user_id },
      game_server_id_{ std::move(game_server_id) },
      session_key_{ generate_token() },
      dirty_{ false },
      player_state_{ player_state_type::loading },
      components_{ std::make_shared<player_role_component>(this),
                   std::make_shared<player_online_component>(this) },
      resource_loader_{ resource_loader }
{
    check();
}

void celeritas::player_state::set_dirty()
{
    dirty_ = true;
}

void celeritas::player_state::set_player_state_type(player_state_type player_state_type)
{
    player_state_ = player_state_type;
}

celeritas::player_state_type celeritas::player_state::get_player_state_type() const
{
    return player_state_;
}

celeritas::player_state::void_awaitable_type celeritas::player_state::on_load_db()
{
    for (const auto& element : components_)
    {
        co_await element->on_load_db();
    }
}

celeritas::player_state::void_awaitable_type celeritas::player_state::on_db_analysis()
{
    for (const auto& element : components_)
    {
        co_await element->on_db_analysis();
    }
}

celeritas::player_state::void_awaitable_type celeritas::player_state::on_dependencies_ready()
{
    for (const auto& element : components_)
    {
        co_await element->on_dependencies_ready();
    }
}

celeritas::player_state::void_awaitable_type celeritas::player_state::send_initial_sync()
{
    for (const auto& element : components_)
    {
        co_await element->send_initial_sync();
    }
}

celeritas::player_state::void_awaitable_type celeritas::player_state::on_login()
{
    for (const auto& element : components_)
    {
        co_await element->on_login();
    }
}

celeritas::player_state::void_awaitable_type celeritas::player_state::on_logout()
{
    for (const auto& element : components_)
    {
        co_await element->on_logout();
    }
}

int64_t celeritas::player_state::get_user_id() const noexcept
{
    return user_id_;
}

std::string celeritas::player_state::get_game_server_id() const
{
    return game_server_id_;
}

std::string celeritas::player_state::generate_token()
{
    boost::uuids::random_generator generator{};
    const auto uuid = generator();

    return boost::uuids::to_string(uuid);
}

std::string celeritas::player_state::get_session_key()
{
    return session_key_;
}

void celeritas::player_state::check()
{
    for (auto index = 0; index < static_cast<int>(player_component_type::max_component); ++index)
    {
        if (static_cast<int>(components_.at(index)->get_player_component_type()) != index)
        {
            throw celeritas_error{ "component type error,i = {}", index };
        }
    }
}
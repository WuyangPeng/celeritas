#include "player_red_dot_component.h"
#include "config/game/game_tables.h"
#include "database/database_pool_base.h"
#include "detail/calculate_red_dot.h"
#include "player/component/player_state.h"

celeritas::player_red_dot_component::player_red_dot_component(player_state* player_state) noexcept
    : base_type{ get_player_component_type(), player_state },
      database_{ player_state, this },
      document_{},
      node_{ player_state, &document_ }
{
}

celeritas::player_component::void_awaitable_type celeritas::player_red_dot_component::on_load_db()
{
    co_await load_user_red_dots();

    set_red_dots();

    node_.set_red_dot_node();
}

celeritas::player_component::void_awaitable_type celeritas::player_red_dot_component::on_dependencies_ready()
{
    calculate_red_dot();

    co_return;
}

void celeritas::player_red_dot_component::add_red_dot(const red_dot_type red_dot_type)
{
    add_red_dot(red_dot_type, 1);
}

void celeritas::player_red_dot_component::add_red_dot(const red_dot_type red_dot_type, const int value)
{
    if (node_.add_red_dot(red_dot_type, value))
    {
        update_document();
    }
}

void celeritas::player_red_dot_component::reduce_red_dot(const red_dot_type red_dot_type)
{
    reduce_red_dot(red_dot_type, 1);
}

void celeritas::player_red_dot_component::reduce_red_dot(const red_dot_type red_dot_type, const int value)
{
    if (node_.reduce_red_dot(red_dot_type, value))
    {
        update_document();
    }
}

void celeritas::player_red_dot_component::change_red_dot(const red_dot_type red_dot_type)
{
    if (node_.change_red_dot(red_dot_type))
    {
        update_document();
    }
}

int celeritas::player_red_dot_component::get_red_dot_value(red_dot_type red_dot_type) const
{
    return node_.get_red_dot_value(red_dot_type);
}

celeritas::player_component::void_awaitable_type celeritas::player_red_dot_component::save_db()
{
    co_return co_await database_.save_db();
}

bool celeritas::player_red_dot_component::is_modify() const
{
    return database_.is_modify();
}

celeritas::player_component::void_awaitable_type celeritas::player_red_dot_component::load_user_red_dots()
{
    co_return co_await database_.load_user_red_dots();
}

void celeritas::player_red_dot_component::set_red_dots()
{
    document_.set_red_dots(database_.get_red_dots());
}

void celeritas::player_red_dot_component::calculate_red_dot()
{
    if (node_.calculate_red_dot())
    {
        update_document();
    }
}

void celeritas::player_red_dot_component::update_document()
{
    database_.set_red_dots(document_.get_red_dots());
}


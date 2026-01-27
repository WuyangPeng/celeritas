#include "player_red_dot_component.h"
#include "common/core/enum_cast.h"
#include "common/logging/logger.h"
#include "config/game/game_tables.h"
#include "database/pool/database_pool_base.h"
#include "detail/calculate_red_dot.h"
#include "initializer/initializer_constant.h"
#include "player/component/player_state.h"
#include "proto/celeritas.pb.h"

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

        send_red_dot_response(red_dot_type);
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

        send_red_dot_response(red_dot_type);
    }
}

void celeritas::player_red_dot_component::change_red_dot(const red_dot_type red_dot_type)
{
    if (node_.change_red_dot(red_dot_type))
    {
        update_document();

        send_red_dot_response(red_dot_type);
    }
}

int celeritas::player_red_dot_component::get_red_dot_value(const red_dot_type red_dot_type) const
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

celeritas::player_component::void_awaitable_type celeritas::player_red_dot_component::send_initial_sync()
{
    send_red_dot_response();

    co_return;
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

        // 这是玩家上线操作，不需要发送消息
    }
}

void celeritas::player_red_dot_component::update_document()
{
    database_.set_red_dots(document_.get_red_dots());
}

void celeritas::player_red_dot_component::send_red_dot_response()
{
    auto* player_state = get_player_state();

    const header header{ player_state->get_user_id() };

    proto::celeritas response{};
    auto* red_dot_response = response.mutable_celeritas_response()->mutable_client()->mutable_player()->mutable_red_dot()->mutable_red_dot();
    red_dot_response->set_is_login(true);
    for (const auto& [red_dot_type, value] : node_)
    {
        auto* node = red_dot_response->add_node();
        node->set_red_dot_type(enum_cast_underlying(red_dot_type));
        node->set_value(value->get_value());
    }

    if (!player_state->write(gateway_type.data(), player_state->get_instance_id(), header, response))
    {
        LOG_CHANNEL(player_channel, error) << "send message error.";
    }
}

void celeritas::player_red_dot_component::send_red_dot_response(const red_dot_type red_dot_type)
{
    auto* player_state = get_player_state();

    const header header{ player_state->get_user_id() };

    proto::celeritas response{};
    auto* red_dot_response = response.mutable_celeritas_response()->mutable_client()->mutable_player()->mutable_red_dot()->mutable_red_dot();
    red_dot_response->set_is_login(false);

    auto* node = red_dot_response->add_node();
    node->set_red_dot_type(enum_cast_underlying(red_dot_type));
    node->set_value(node_.get_red_dot_value(red_dot_type));

    if (!player_state->write(gateway_type.data(), player_state->get_instance_id(), header, response))
    {
        LOG_CHANNEL(player_channel, error) << "send message error.";
    }
}


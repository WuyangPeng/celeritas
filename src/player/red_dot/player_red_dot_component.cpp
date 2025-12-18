#include "player_red_dot_component.h"
#include "red_dot_node.h"
#include "common/celeritas_error.h"
#include "config/database_type.h"
#include "config/game_config/container_config.h"
#include "config/game_config/game_config.h"
#include "config/game_config/game_tables.h"
#include "config/game_config/red_config.h"
#include "config/game_config/red_dot_type.h"
#include "database/database_pool_base.h"
#include "detail/calculate_red_dot.h"
#include "player/component/player_state.h"
#include "player/user/player_user_component.h"

celeritas::player_red_dot_component::player_red_dot_component(player_state* player_state) noexcept
    : base_type{ get_player_component_type(), player_state }, user_red_dots_{}, red_dots_{}, red_dot_node_{}
{
}

celeritas::player_component::void_awaitable_type celeritas::player_red_dot_component::on_load_db()
{
    co_await load_user_red_dots();

    set_red_dots();

    set_red_dot_node();
}

celeritas::player_component::void_awaitable_type celeritas::player_red_dot_component::on_dependencies_ready()
{
    calculate_red_dot_by_database();

    calculate_children_red_dot();

    calculate_parent_red_dot();

    co_return;
}

celeritas::player_component::void_awaitable_type celeritas::player_red_dot_component::load_user_red_dots()
{
    const auto mongo_player_pool = get_mongo_player_database_pool();
    const auto player_user = get_player_state()->get_component<player_user_component>();
    const auto user_id = player_user->get_user_id();

    if (const auto database_entity_change = co_await mongo_player_pool->select_one(user_red_dots::get_select(database_type::mongo, user_id), user_red_dots::get_database_field_container()))
    {
        user_red_dots_ = user_red_dots{ *database_entity_change };
    }
    else
    {
        user_red_dots_ = user_red_dots{ database_type::mongo, user_id };
    }
}

void celeritas::player_red_dot_component::set_red_dots()
{
    for (const auto& element : user_red_dots_->get_red_dots())
    {
        auto red_dots = red_dots::from_json_string(element);
        red_dots_.emplace(red_dots.get_node_id(), red_dots);
    }
}

void celeritas::player_red_dot_component::set_red_dot_node()
{
    const auto& game_tables = game_config::get_instance().get_game_tables();
    const auto& container = game_tables->get_red_container()->get_container();
    for (const auto& type : container | std::views::keys)
    {
        red_dot_node_.emplace(type, std::make_shared<red_dot_node>(type));
    }

    for (const auto& [red_dot_type, element] : container)
    {
        auto self_iter = red_dot_node_.find(red_dot_type);
        if (self_iter == red_dot_node_.cend())
        {
            throw celeritas_error{ "set red dot node error,id = {}", static_cast<int>(red_dot_type) };
        }

        if (const auto parent_node_id = element->get_parent_node_id();
            parent_node_id != red_dot_type::null)
        {
            auto parent_iter = red_dot_node_.find(parent_node_id);
            if (parent_iter == red_dot_node_.cend())
            {
                throw celeritas_error{ "set red dot node error,parent id = {}", static_cast<int>(parent_node_id) };
                continue;
            }

            self_iter->second->set_parent_node(parent_iter->second);
            parent_iter->second->add_child(self_iter->second);
        }
    }
}

void celeritas::player_red_dot_component::calculate_red_dot_by_database()
{
    for (const auto& [red_type, red_dots] : red_dots_)
    {
        if (red_dots.is_state())
        {
            if (auto iter = red_dot_node_.find(red_type);
                iter != red_dot_node_.cend())
            {
                iter->second->set_value(0);
            }
        }
    }
}

void celeritas::player_red_dot_component::calculate_children_red_dot()
{
    for (const auto& [red_type, red_dot_node] : red_dot_node_)
    {
        if (red_dot_node->is_child())
        {
            const auto calculate_red_dot = calculate_red_dot::create(red_type, get_player_state());

            red_dot_node->set_value(calculate_red_dot->get_red_dot_value());
        }
    }
}

void celeritas::player_red_dot_component::calculate_parent_red_dot()
{
    for (const auto& element : red_dot_node_)
    {
        if (!element.second->is_child())
        {
        }
    }
}


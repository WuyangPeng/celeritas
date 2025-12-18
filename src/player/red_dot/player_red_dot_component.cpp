#include "player_red_dot_component.h"
#include "red_dot_node.h"
#include "common/celeritas_error.h"
#include "common/time_helper.h"
#include "config/database_type.h"
#include "config/game_config/container_config.tpp"
#include "config/game_config/game_config.h"
#include "config/game_config/game_tables.h"
#include "config/game_config/red_config.h"
#include "config/game_config/red_dot_type.h"
#include "database/database_pool_base.h"
#include "detail/calculate_red_dot.h"
#include "player/component/player_state.h"
#include "player/user/player_user_component.h"

celeritas::player_red_dot_component::player_red_dot_component(player_state* player_state) noexcept
    : base_type{ get_player_component_type(), player_state },
      database_{ player_state, this },
      red_dots_{},
      red_dot_node_{}
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
    calculate_red_dot();

    co_return;
}

void celeritas::player_red_dot_component::add_red_dot(red_dot_type red_dot_type)
{
    add_red_dot(red_dot_type, 1);
}

void celeritas::player_red_dot_component::add_red_dot(red_dot_type red_dot_type, int value)
{
    const auto iter = red_dot_node_.find(red_dot_type);
    if (iter == red_dot_node_.end())
    {
        throw celeritas_error{ "red_dot_type not found,red dot type = {}", static_cast<int>(red_dot_type) };
    }

    if (!iter->second->is_child())
    {
        throw celeritas_error{ "red_dot_type is not child,red dot type = {}", static_cast<int>(red_dot_type) };
    }

    iter->second->add_value(value);

    auto change = false;
    if (iter->second->is_save_database())
    {
        if (const auto red_dots_iter = red_dots_.find(red_dot_type);
            red_dots_iter == red_dots_.cend())
        {
            red_dots_.emplace(red_dot_type, red_dots{ red_dot_type, false });
            change = true;
        }
    }

    auto parent = iter->second->get_parent();
    while (parent != nullptr)
    {
        (*parent)->add_value(value);
        parent = (*parent)->get_parent();
    }

    if (change)
    {
        update_document();
    }
}

void celeritas::player_red_dot_component::reduce_red_dot(red_dot_type red_dot_type)
{
    reduce_red_dot(red_dot_type, 1);
}

void celeritas::player_red_dot_component::reduce_red_dot(red_dot_type red_dot_type, int value)
{
    const auto iter = red_dot_node_.find(red_dot_type);
    if (iter == red_dot_node_.end())
    {
        throw celeritas_error{ "red_dot_type not found,red dot type = {}", static_cast<int>(red_dot_type) };
    }

    if (!iter->second->is_child())
    {
        throw celeritas_error{ "red_dot_type is not child,red dot type = {}", static_cast<int>(red_dot_type) };
    }

    iter->second->reduce_value(value);

    const auto finish = iter->second->get_value() <= 0;

    auto change = false;
    if (iter->second->is_save_database())
    {
        const auto red_dots_iter = red_dots_.find(red_dot_type);
        if (red_dots_iter == red_dots_.cend())
        {
            red_dots_.emplace(red_dot_type, red_dots{ red_dot_type, finish });
            change = true;
        }
        else if (finish)
        {
            red_dots_iter->second.set_state(true);
            change = true;
        }
    }

    auto parent = iter->second->get_parent();
    while (parent != nullptr)
    {
        (*parent)->reduce_value(value);
        parent = (*parent)->get_parent();
    }

    if (change)
    {
        update_document();
    }
}

void celeritas::player_red_dot_component::change_red_dot(red_dot_type red_dot_type)
{
    const auto iter = red_dot_node_.find(red_dot_type);
    if (iter == red_dot_node_.end())
    {
        throw celeritas_error{ "red_dot_type not found,red dot type = {}", static_cast<int>(red_dot_type) };
    }

    if (!iter->second->is_child())
    {
        throw celeritas_error{ "red_dot_type is not child,red dot type = {}", static_cast<int>(red_dot_type) };
    }

    const auto calculate_red_dot = calculate_red_dot::create(red_dot_type, get_player_state());
    const auto value = calculate_red_dot->get_red_dot_value();
    const auto old_value = iter->second->get_value();
    iter->second->set_value(value);
    const auto difference = value - old_value;
    const auto finish = iter->second->get_value() <= 0;

    auto change = false;
    if (iter->second->is_save_database())
    {
        const auto red_dots_iter = red_dots_.find(red_dot_type);
        if (red_dots_iter == red_dots_.cend())
        {
            red_dots_.emplace(red_dot_type, red_dots{ red_dot_type, finish });
            change = true;
        }
        else if (finish)
        {
            red_dots_iter->second.set_state(true);
            change = true;
        }
    }

    auto parent = iter->second->get_parent();
    while (parent != nullptr)
    {
        (*parent)->add_value(difference);
        parent = (*parent)->get_parent();
    }

    if (change)
    {
        update_document();
    }
}

celeritas::player_component::void_awaitable_type celeritas::player_red_dot_component::load_user_red_dots()
{
    co_return co_await database_.load_user_red_dots();
}

void celeritas::player_red_dot_component::set_red_dots()
{
    for (const auto& element : database_.get_red_dots())
    {
        auto red_dots = red_dots::from_json_string(element);
        red_dots_.emplace(red_dots.get_node_id(), red_dots);
    }
}

void celeritas::player_red_dot_component::set_red_dot_node()
{
    const auto& game_tables = game_config::get_instance().get_game_tables();
    const auto& container = game_tables->get_red_container()->get_container();
    for (const auto& element : container)
    {
        red_dot_node_.emplace(element.first, std::make_shared<red_dot_node>(element.first, element.second->is_save_database()));
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

void celeritas::player_red_dot_component::calculate_red_dot()
{
    auto change = false;
    for (const auto& [red_dot_type, red_dot_node] : red_dot_node_)
    {
        if (red_dot_node->is_child())
        {
            if (red_dot_node->is_save_database())
            {
                const auto iter = red_dots_.find(red_dot_type);
                if (iter == red_dots_.cend())
                {
                    const auto calculate_red_dot = calculate_red_dot::create(red_dot_type, get_player_state());
                    const auto value = calculate_red_dot->get_red_dot_value();

                    if (value <= 0)
                    {
                        red_dots_.emplace(red_dot_type, red_dots{ red_dot_type, value <= 0 });
                        change = true;
                    }
                }

                if (iter->second.is_state())
                {
                    red_dot_node->set_value(0);
                    continue;
                }
            }

            const auto calculate_red_dot = calculate_red_dot::create(red_dot_type, get_player_state());
            const auto value = calculate_red_dot->get_red_dot_value();
            red_dot_node->set_value(value);

            auto parent = red_dot_node->get_parent();
            while (parent != nullptr)
            {
                (*parent)->add_value(red_dot_node->get_value());
                parent = (*parent)->get_parent();
            }
        }
    }

    if (change)
    {
        update_document();
    }
}

void celeritas::player_red_dot_component::update_document()
{
    traits::document_array_type documents{};
    for (auto& element : red_dots_ | std::views::values)
    {
        documents.emplace_back(element.to_json_string());
    }

    database_.set_red_dots(documents);
}


#include "calculate_red_dot.h"
#include "player_red_dot_node.h"
#include "common/core/celeritas_error.h"
#include "config/game/game_config.h"
#include "config/game/game_tables.h"

#include <ranges>

celeritas::player_red_dot_node::player_red_dot_node(player_state* player_state, player_red_dot_document* player_red_dot_document)
    : player_state_{ player_state }, player_red_dot_document_{ player_red_dot_document }, red_dot_node_{}
{
}

bool celeritas::player_red_dot_node::add_red_dot(const red_dot_type red_dot_type, const int value) const
{
    const auto red_dot_node = get_child_red_dot(red_dot_type);

    red_dot_node->add_value(value);

    const auto change = insert_document(red_dot_node);

    add_parent_value(red_dot_node, value);

    return change;
}

bool celeritas::player_red_dot_node::reduce_red_dot(const red_dot_type red_dot_type, const int value) const
{
    const auto red_dot_node = get_child_red_dot(red_dot_type);

    red_dot_node->reduce_value(value);

    const auto change = insert_document(red_dot_node);

    add_parent_value(red_dot_node, -value);

    return change;
}

bool celeritas::player_red_dot_node::change_red_dot(const red_dot_type red_dot_type) const
{
    const auto red_dot_node = get_child_red_dot(red_dot_type);

    const auto calculate_red_dot = calculate_red_dot::create(red_dot_type, player_state_);
    const auto new_value = calculate_red_dot->get_red_dot_value();

    const auto old_value = red_dot_node->get_value();
    red_dot_node->set_value(new_value);
    const auto difference = new_value - old_value;

    const auto change = set_document(red_dot_node);

    add_parent_value(red_dot_node, difference);

    return change;
}

void celeritas::player_red_dot_node::set_red_dot_node()
{
    const auto& game_tables = game_config::get_instance().get_game_tables();
    const auto& container = game_tables->get_tables()->red_dot_config_container;

    init_red_dot_node(container);
    set_red_dot_node_association(container);
}

bool celeritas::player_red_dot_node::calculate_red_dot()
{
    auto change = false;
    for (const auto& red_dot_node : red_dot_node_ | std::views::values)
    {
        if (red_dot_node->is_child())
        {
            if (calculate_red_dot(red_dot_node))
            {
                change = true;
            }
        }
    }

    return change;
}

int celeritas::player_red_dot_node::get_red_dot_value(const red_dot_type red_dot_type) const
{
    const auto iter = red_dot_node_.find(red_dot_type);
    if (iter == red_dot_node_.end())
    {
        return 0;
    }

    return iter->second->get_value();
}

celeritas::player_red_dot_node::red_dot_node_container_const_iter celeritas::player_red_dot_node::begin() const
{
    return red_dot_node_.begin();
}

celeritas::player_red_dot_node::red_dot_node_container_const_iter celeritas::player_red_dot_node::end() const
{
    return red_dot_node_.end();
}

celeritas::player_red_dot_node::red_dot_node_shared_ptr celeritas::player_red_dot_node::get_child_red_dot(red_dot_type red_dot_type) const
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

    return iter->second;
}

bool celeritas::player_red_dot_node::insert_document(const red_dot_node_shared_ptr& red_dot_node) const
{
    const auto finish = red_dot_node->get_value() <= 0;

    if (red_dot_node->is_save_database())
    {
        if (player_red_dot_document_->insert_red_dot(red_dot_node->get_red_dot_type(), finish))
        {
            return true;
        }
    }

    return false;
}

bool celeritas::player_red_dot_node::set_document(const red_dot_node_shared_ptr& red_dot_node) const
{
    const auto finish = red_dot_node->get_value() <= 0;

    if (red_dot_node->is_save_database())
    {
        if (player_red_dot_document_->set_red_dot(red_dot_node->get_red_dot_type(), finish))
        {
            return true;
        }
    }

    return false;
}

void celeritas::player_red_dot_node::add_parent_value(const red_dot_node_shared_ptr& red_dot_node, const int value)
{
    auto parent = red_dot_node->get_parent();
    while (parent)
    {
        (*parent)->add_value(value);
        parent = (*parent)->get_parent();
    }
}

void celeritas::player_red_dot_node::init_red_dot_node(const config::game::red_dot_config_container& container)
{
    for (const auto& [red_dot_type, red_config] : container.getDataMap())
    {
        red_dot_node_.emplace(red_dot_type, std::make_shared<red_dot_node>(red_dot_type, red_config->saveDatabase));
    }
}

void celeritas::player_red_dot_node::set_red_dot_node_association(const config::game::red_dot_config_container& container)
{
    for (const auto& element : container.getDataMap() | std::views::values)
    {
        set_red_dot_node_association(element);
    }
}

void celeritas::player_red_dot_node::set_red_dot_node_association(const const_red_dot_config_shared_ptr& red_dot_config)
{
    const auto self_iter = red_dot_node_.find(red_dot_config->id);
    if (self_iter == red_dot_node_.cend())
    {
        throw celeritas_error{ "set red dot node error,id = {}", static_cast<int>(red_dot_config->id) };
    }

    if (const auto parent_node_id = red_dot_config->parentNodeId;
        parent_node_id != red_dot_type::none)
    {
        const auto parent_iter = red_dot_node_.find(parent_node_id);
        if (parent_iter == red_dot_node_.cend())
        {
            throw celeritas_error{ "set red dot node error,parent id = {}", static_cast<int>(parent_node_id) };
        }

        self_iter->second->set_parent_node(parent_iter->second);
        parent_iter->second->add_child(self_iter->second);
    }
}

bool celeritas::player_red_dot_node::calculate_red_dot(const red_dot_node_shared_ptr& red_dot_node) const
{
    auto change = false;
    if (red_dot_node->is_save_database())
    {
        change = calculate_red_dot_by_save_database(red_dot_node);
    }

    const auto value = get_red_dot_value(red_dot_node);

    red_dot_node->set_value(value);
    add_parent_value(red_dot_node, value);

    return change;
}

bool celeritas::player_red_dot_node::calculate_red_dot_by_save_database(const red_dot_node_shared_ptr& red_dot_node) const
{
    auto change = false;
    if (!player_red_dot_document_->is_exist(red_dot_node->get_red_dot_type()))
    {
        if (const auto value = get_red_dot_value(red_dot_node);
            value <= 0)
        {
            if (player_red_dot_document_->insert_red_dot(red_dot_node->get_red_dot_type(), true))
            {
                change = true;
            }
        }
        else
        {
            red_dot_node->set_value(value);
            add_parent_value(red_dot_node, value);

            return change;
        }
    }

    if (player_red_dot_document_->is_state(red_dot_node->get_red_dot_type()))
    {
        red_dot_node->set_value(0);
    }

    return change;
}

int celeritas::player_red_dot_node::get_red_dot_value(const red_dot_node_shared_ptr& red_dot_node) const
{
    const auto calculate_red_dot = calculate_red_dot::create(red_dot_node->get_red_dot_type(), player_state_);

    return calculate_red_dot->get_red_dot_value();
}


#include "calculate_red_dot.h"
#include "player_red_dot_node.h"
#include "common/celeritas_error.h"
#include "config/game_config/game_config.h"
#include "config/game_config/game_tables.h"
#include "config/game_config/red_config.h"

celeritas::player_red_dot_node::player_red_dot_node(player_state* player_state, player_red_dot_document* player_red_dot_document)
    : player_state_{ player_state }, player_red_dot_document_{ player_red_dot_document }, red_dot_node_{}
{
}

bool celeritas::player_red_dot_node::add_red_dot(red_dot_type red_dot_type, const int value)
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
    const auto finish = iter->second->get_value() <= 0;
    auto change = false;
    if (iter->second->is_save_database())
    {
        if (player_red_dot_document_->insert_red_dot(red_dot_type, finish))
        {
            change = true;
        }
    }

    auto parent = iter->second->get_parent();
    while (parent != nullptr)
    {
        (*parent)->add_value(value);
        parent = (*parent)->get_parent();
    }

    return change;
}

bool celeritas::player_red_dot_node::reduce_red_dot(red_dot_type red_dot_type, const int value)
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
        if (player_red_dot_document_->set_red_dot(red_dot_type, finish))
        {
            change = true;
        }
    }

    auto parent = iter->second->get_parent();
    while (parent != nullptr)
    {
        (*parent)->reduce_value(value);
        parent = (*parent)->get_parent();
    }

    return change;
}

bool celeritas::player_red_dot_node::change_red_dot(red_dot_type red_dot_type)
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

    const auto calculate_red_dot = calculate_red_dot::create(red_dot_type, player_state_);
    const auto value = calculate_red_dot->get_red_dot_value();
    const auto old_value = iter->second->get_value();
    iter->second->set_value(value);
    const auto difference = value - old_value;
    const auto finish = iter->second->get_value() <= 0;

    auto change = false;
    if (iter->second->is_save_database())
    {
        if (player_red_dot_document_->set_red_dot(red_dot_type, finish))
        {
            change = true;
        }
    }

    auto parent = iter->second->get_parent();
    while (parent != nullptr)
    {
        (*parent)->add_value(difference);
        parent = (*parent)->get_parent();
    }

    return change;
}

void celeritas::player_red_dot_node::set_red_dot_node()
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

bool celeritas::player_red_dot_node::calculate_red_dot()
{
    auto change = false;
    for (const auto& [red_dot_type, red_dot_node] : red_dot_node_)
    {
        if (red_dot_node->is_child())
        {
            if (red_dot_node->is_save_database())
            {
                if (!player_red_dot_document_->is_exist(red_dot_type))
                {
                    const auto calculate_red_dot = calculate_red_dot::create(red_dot_type, player_state_);

                    if (const auto value = calculate_red_dot->get_red_dot_value();
                        value <= 0)
                    {
                        if (player_red_dot_document_->insert_red_dot(red_dot_type, true))
                        {
                            change = true;
                        }
                    }
                }

                if (player_red_dot_document_->is_state(red_dot_type))
                {
                    red_dot_node->set_value(0);
                    continue;
                }
            }

            const auto calculate_red_dot = calculate_red_dot::create(red_dot_type, player_state_);
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

    return change;
}


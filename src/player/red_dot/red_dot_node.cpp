#include "player/red_dot/red_dot_node.h"

celeritas::red_dot_node::red_dot_node(const red_dot_type type, bool save_database)
    : type_{ type }, value_{ 0 }, parent_{}, children_{}, save_database_{ save_database }
{
}

void celeritas::red_dot_node::set_value(const int value)
{
    value_ = value;
}

void celeritas::red_dot_node::add_value(const int value)
{
    value_ += value;
}

void celeritas::red_dot_node::reduce_value(int value)
{
    value_ -= value;
}

int celeritas::red_dot_node::get_value() const
{
    return value_;
}

void celeritas::red_dot_node::set_parent_node(const red_dot_node_shared_ptr& parent_node)
{
    parent_ = parent_node;
}

void celeritas::red_dot_node::add_child(const red_dot_node_shared_ptr& child)
{
    children_.emplace_back(child);
}

bool celeritas::red_dot_node::is_child() const
{
    return children_.empty();
}

bool celeritas::red_dot_node::is_save_database() const
{
    return save_database_;
}

celeritas::red_dot_node::red_dot_node_optional celeritas::red_dot_node::get_parent() const
{
    if (const auto result = parent_.lock();
        result != nullptr)
    {
        return result;
    }

    return std::nullopt;
}
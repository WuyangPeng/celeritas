#include "player/red_dot/red_dot_node.h"

celeritas::red_dot_node::red_dot_node(const red_dot_type type)
    : type_{ type }, value_{ 0 }, parent_{}, children_{}
{
}

void celeritas::red_dot_node::set_value(const int value)
{
    value_ = value;
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
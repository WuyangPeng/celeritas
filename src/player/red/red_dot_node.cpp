#include "player/red/red_dot_node.h"

celeritas::red_dot_node::red_dot_node(const red_type type)
    : type_{ type }, value_{ 0 }, parent_{}, children_{}
{
}
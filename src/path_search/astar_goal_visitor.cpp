#include "astar_goal_visitor.h"
#include "found_goal.h"

celeritas::astar_goal_visitor::astar_goal_visitor(vertex goal)
    : goal_vertex_{ std::move(goal) }
{
}

void celeritas::astar_goal_visitor::examine_vertex(vertex u, const grid_graph& g)
{
    if (u == goal_vertex_)
    {
        throw found_goal{};
    }
}
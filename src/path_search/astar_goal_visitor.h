#pragma once

#include "path_search_base.h"

#include <boost/graph/astar_search.hpp>

namespace celeritas
{
    class astar_goal_visitor : public boost::default_astar_visitor
    {
    public:
        using class_type = astar_goal_visitor;

        explicit astar_goal_visitor(vertex goal);

        // 当一个节点被“发现”时调用
        void examine_vertex(vertex u, const grid_graph& g);

    private:
        vertex goal_vertex_;
    };
}
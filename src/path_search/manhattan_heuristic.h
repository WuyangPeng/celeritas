#pragma once

#include "path_search_base.h"

#include <boost/graph/astar_search.hpp>

namespace celeritas
{
    class manhattan_heuristic : public boost::astar_heuristic<grid_graph, double>
    {
    public:
        using class_type = manhattan_heuristic;
        using base_type = astar_heuristic;

        explicit manhattan_heuristic(vertex goal);

        [[nodiscard]] double operator()(vertex u);

    private:
        vertex goal_vertex_;
    };
}
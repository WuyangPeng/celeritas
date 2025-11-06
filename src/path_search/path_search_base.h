#pragma once

#include <boost/graph/grid_graph.hpp>

namespace celeritas
{
    using grid_graph = boost::grid_graph<2>;
    using vertex = grid_graph::vertex_descriptor;
    using edge = grid_graph::edge_descriptor;
}
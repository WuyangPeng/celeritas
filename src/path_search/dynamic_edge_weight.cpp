#include "dynamic_edge_weight.h"

#include <boost/graph/grid_graph.hpp>
#include <boost/graph/graph_traits.hpp>

celeritas::dynamic_edge_weight::dynamic_edge_weight(map_grid_shared_ptr map_grid)
    : map_grid_{ std::move(map_grid) }
{
}

double celeritas::get(const dynamic_edge_weight& dynamic_edge_weight, const edge& edge)
{
    const auto& source = edge.first;
    const auto& target = edge.second;

    // 1. 基础移动成本 (例如，直线1.0，对角线1.414)
    auto cost = (source[0] != target[0] && source[1] != target[1]) ? 1.414 : 1.0;

    // 2. 动态障碍物/地形成本检查
    // 我们只检查目标节点，因为只有到达目标节点时才会发生“地形成本”
    if (const auto& obstacle_map = dynamic_edge_weight.get_map_grid()->get_obstacles();
        obstacle_map.contains(target))
    {
        // 如果目标是障碍物，返回一个巨大的值（例如：INF），但 BGL 的 A* 通常会通过 Visitor 或在生成邻居时被排除
        if (obstacle_map.at(target) == std::numeric_limits<double>::infinity())
        {
            return std::numeric_limits<double>::infinity();
        }
        // 否则，加上地形的额外成本
        cost += obstacle_map.at(target);
    }
    return cost;
}

celeritas::dynamic_edge_weight::map_grid_shared_ptr celeritas::dynamic_edge_weight::get_map_grid() const
{
    return map_grid_;
}
#include "astar_goal_visitor.h"
#include "astar_search.h"
#include "dynamic_edge_weight.h"
#include "found_goal.h"
#include "manhattan_heuristic.h"

celeritas::astar_search::astar_search(map_grid_shared_ptr map_grid)
    : map_grid_{ std::move(map_grid) }
{
}

celeritas::astar_search::path_result_type celeritas::astar_search::find_path(const vertex& start, const vertex& goal)
{
    grid_graph g{ map_grid_->get_dimensions() };

    // 准备存储结果的 map
    std::unordered_map<vertex, vertex, vertex_hash> predecessor{};
    std::unordered_map<vertex, double, vertex_hash> distance{};

    // 设置起点
    distance[start] = 0.0;

    // 1. 设置权重映射
    dynamic_edge_weight weight_map{ map_grid_ };

    // 2. 设置启发式
    manhattan_heuristic heuristic{ goal };

    try
    {
        // 执行 A* 搜索
        boost::astar_search(g,
                            start,
                            heuristic,
                            boost::predecessor_map(boost::make_assoc_property_map(predecessor))
                            .distance_map(boost::make_assoc_property_map(distance))
                            .weight_map(weight_map)
                            .visitor(astar_goal_visitor(goal))
            );
    }
    catch (const found_goal&)
    {
        // 找到路径，回溯
        std::vector<vertex> path;
        vertex curr = goal;
        while (curr != start)
        {
            path.push_back(curr);
            curr = predecessor[curr];
        }
        path.push_back(start);
        std::reverse(path.begin(), path.end());
        return path;
    }

    return {}; // 未找到路径
}
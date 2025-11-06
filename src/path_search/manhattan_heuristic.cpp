#include "manhattan_heuristic.h"

celeritas::manhattan_heuristic::manhattan_heuristic(vertex goal)
    : goal_vertex_{ std::move(goal) }
{
}

double celeritas::manhattan_heuristic::operator()(vertex u)
{
    const auto dx = std::abs(static_cast<double>(u[0]) - goal_vertex_[0]);
    const auto dy = std::abs(static_cast<double>(u[1]) - goal_vertex_[1]);

    const auto straight_cost = 1.0;
    const auto diagonal_cost = 1.414; // 使用 sqrt(2) 的近似值，与 dynamic_edge_weight.cpp 中设定的成本一致

    const auto min_diff = std::min(dx, dy);
    const auto max_diff = std::max(dx, dy);

    // 八方向距离公式:
    // cost = (对角线步数 * 对角线成本) + (直线步数 * 直线成本)
    return (min_diff * diagonal_cost) + ((max_diff - min_diff) * straight_cost);
}
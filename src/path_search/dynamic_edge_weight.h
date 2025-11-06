#pragma once

#include "map_grid.h"

namespace celeritas
{
    class dynamic_edge_weight
    {
    public:
        using class_type = dynamic_edge_weight;
        using dynamic_obstacles_type = std::unordered_map<vertex, double, vertex_hash>;
        using key_type = edge;
        using value_type = double;
        using reference = value_type;
        using category = boost::readable_property_map_tag;
        using map_grid_shared_ptr = std::shared_ptr<map_grid>;

        explicit dynamic_edge_weight(map_grid_shared_ptr map_grid);

        [[nodiscard]] map_grid_shared_ptr get_map_grid() const;

    private:
        map_grid_shared_ptr map_grid_;
    };

    [[nodiscard]] double get(const dynamic_edge_weight& dynamic_edge_weight, const edge& edge);
}
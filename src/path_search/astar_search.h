#pragma once

#include "map_grid.h"

#include <memory>

namespace celeritas
{
    class astar_search
    {
    public:
        using class_type = astar_search;
        using map_grid_shared_ptr = std::shared_ptr<map_grid>;
        using path_result_type = std::vector<vertex>;

        explicit astar_search(map_grid_shared_ptr map_grid);

        [[nodiscard]] path_result_type find_path(const vertex& start, const vertex& goal);

    private:
        map_grid_shared_ptr map_grid_;
    };
}

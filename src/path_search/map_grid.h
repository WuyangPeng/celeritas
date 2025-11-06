#pragma once

#include "path_search_base.h"
#include "vertex_hash.h"

#include <unordered_map>

namespace celeritas
{
    class map_grid
    {
    public:
        using class_type = map_grid;
        using dynamic_obstacles_type = std::unordered_map<vertex, double, vertex_hash>;
        using diemension_type = boost::array<std::size_t, 2>;

        map_grid(size_t width, size_t height);

        [[nodiscard]] size_t get_width() const noexcept;

        [[nodiscard]] size_t get_height() const noexcept;

        [[nodiscard]] const dynamic_obstacles_type& get_obstacles() const noexcept;

        [[nodiscard]] diemension_type get_dimensions() const noexcept;

        [[nodiscard]] size_t get_node_count() const noexcept;

    private:
        size_t width_;
        size_t height_;
        dynamic_obstacles_type dynamic_obstacles_;
    };
}
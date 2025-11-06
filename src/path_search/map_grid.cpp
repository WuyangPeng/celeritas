#include "map_grid.h"

celeritas::map_grid::map_grid(const size_t width, const size_t height)
    : width_{ width }, height_{ height }, dynamic_obstacles_{}
{
}

size_t celeritas::map_grid::get_width() const noexcept
{
    return width_;
}

size_t celeritas::map_grid::get_height() const noexcept
{
    return height_;
}

const celeritas::map_grid::dynamic_obstacles_type& celeritas::map_grid::get_obstacles() const noexcept
{
    return dynamic_obstacles_;
}

celeritas::map_grid::diemension_type celeritas::map_grid::get_dimensions() const noexcept
{
    return diemension_type{ width_, height_ };
}

size_t celeritas::map_grid::get_node_count() const noexcept
{
    return width_ * height_;
}
#pragma once

#include "item_info.h"

#include <vector>

namespace celeritas
{
    class item_container
    {
    public:
        using class_type = item_container;
        using container_type = std::vector<item_info>;
        using container_type_const_iter = container_type::const_iterator;

        void add_item_info(int template_id, int64_t count);

        [[nodiscard]] container_type_const_iter begin() const;

        [[nodiscard]] container_type_const_iter end() const;

    private:
        container_type container_;
    };
}
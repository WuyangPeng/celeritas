#pragma once

#include "path_search_base.h"

namespace celeritas
{
    class vertex_hash
    {
    public:
        using class_type = vertex_hash;

        [[nodiscard]] size_t operator()(const vertex& u) const;
    };
}
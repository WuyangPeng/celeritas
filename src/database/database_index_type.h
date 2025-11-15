#pragma once
namespace celeritas
{
    enum class database_index_type
    {
        null = 0,
        key = 1,
        unique_index = 2,
        index = 3,
        composite_unique_index = 4,
        composite_index = 5,
    };
}
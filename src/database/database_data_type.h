#pragma once

namespace celeritas
{
    enum class database_data_type
    {
        null_type = 0,
        string_type = 1,
        string_array_type = 2,
        int32_type = 3,
        int32_count_type = 4,
        int32_array_type = 5,
        int64_type = 6,
        int64_count_type = 7,
        int64_array_type = 8,
        double_type = 9,
        double_array_type = 10,
        bool_type = 11,
        byte_array_type = 12,
        document_type = 13,
        document_array_type = 14,
    };
}
#pragma once

namespace celeritas
{
    enum class database_change_type
    {
        select_type = 0,
        update_type = 1,
        insert_type = 2,
        delete_type = 3
    };
}
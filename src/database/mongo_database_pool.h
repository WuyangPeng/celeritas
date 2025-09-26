#pragma once


#include "database_pool.h"

namespace celeritas
{
    class mongo_database_pool : public database_pool
    {
    public:
        using class_type = mongo_database_pool;
    };
}

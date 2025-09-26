#pragma once


#include "database_pool.h"

namespace celeritas
{
    class redis_database_pool : public database_pool
    {
    public:
        using class_type = redis_database_pool;
    };
}

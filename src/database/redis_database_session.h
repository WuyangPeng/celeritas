#pragma once

#include "database_session.h"

namespace celeritas
{
    class redis_database_session : public database_session
    {
    public:
        using class_type = redis_database_session;
    };
}

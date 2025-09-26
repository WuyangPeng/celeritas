#pragma once

#include "database_session.h"

namespace celeritas
{
    class mongo_database_session : public database_session
    {
    public:
        using class_type = mongo_database_session;
    };
}

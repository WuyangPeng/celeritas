#pragma once
#include <string>

namespace celeritas
{
    enum class database_type
    {
        unknown,
        mysql,
        mongo,
        redis,
    };
}

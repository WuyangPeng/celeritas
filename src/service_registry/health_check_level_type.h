#pragma once

namespace celeritas
{
    enum class health_check_level_type
    {
        health = 1,
        unhealthy = 2,
        crash = 3,
    };
}
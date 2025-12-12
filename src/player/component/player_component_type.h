#pragma once

namespace celeritas
{
    enum class player_component_type
    {
        user = 0,
        role = 1,
        online = 2,
        finish,
        max_component = finish + 1,
    };
}
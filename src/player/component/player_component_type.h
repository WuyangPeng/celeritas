#pragma once

namespace celeritas
{
    enum class player_component_type
    {
        user = 0,
        role = 1,
        online = 2,
        time = 3,
        item = 4,
        activity = 5,
        develop = 6,
        mail = 7,
        task = 8,
        attribute = 9,
        instance = 10,
        finish,
        max_component = finish + 1,
    };
}
#pragma once

namespace celeritas
{
    enum class player_component_type
    {
        user = 0,
        role = 1,
        online = 2,
        debug = 3,
        time = 4,
        red_dot = 5,
        item = 6,
        develop = 7,
        mail = 8,
        task = 9,
        activity = 10,
        attribute = 11,
        instance = 12,
        finish,
        mock, // 测试使用
        max_component = finish + 2,
    };
}
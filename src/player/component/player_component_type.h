#pragma once

namespace celeritas
{
    enum class player_component_type
    {
        user = 0,
        role = 1,
        online = 2,
        time = 3,
        red_dot = 4,
        item = 5,
        develop = 6,
        mail = 7,
        task = 8,
        activity = 9,
        attribute = 10,
        instance = 11,
        finish,
        mock, // 测试使用
        max_component = finish + 2,
    };
}
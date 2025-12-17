#pragma once

namespace celeritas
{
    enum class player_component_type
    {
        user = 0,
        role = 1,
        online = 2,
        time = 3,
        red = 4,
        item = 5,
        activity = 6,
        develop = 7,
        mail = 8,
        task = 9,
        attribute = 10,
        instance = 11,
        finish,
        mock, // 测试使用
        max_component = finish + 2,
    };
}
#pragma once

namespace celeritas
{
    // 事件监听器优先级（数值越小优先级越高）
    enum class player_event_priority
    {
        highest = 0,
        high = 25,
        normal = 50,
        low = 75,
        lowest = 100
    };
}

#pragma once

namespace celeritas
{
    enum class server_status_type
    {
        normal = 0, // 正常
        busy = 1, // 繁忙
        crowded = 2, // 拥挤
        full = 3, // 爆满
        maintenance = 4, // 维护
    };
}
#pragma once

namespace celeritas
{
    enum class server_mode_type
    {
        formal, // 正式服
        whitelist, // 白名单
        test, // 测试
    };
}
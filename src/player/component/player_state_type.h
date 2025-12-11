#pragma once

namespace celeritas
{
    enum class player_state_type
    {
        loading, // 加载中
        online, // 在线
        disconnected_ghost, // 已断线，状态保留在内存中，等待重连
        logout_pending // 已过期，正在准备安全释放或持久化
    };
}
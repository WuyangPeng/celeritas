#pragma once

namespace celeritas
{
    enum class account_status_type
    {
        normal, // 正常
        muted, // 禁言
        banned, // 封禁
        deleted, // 已删除
    };
}
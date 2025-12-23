#pragma once

namespace celeritas
{
    enum class develop_reset_type
    {
        // 不可重置
        non_resettable = 0,

        // 返还
        refund = 1,

        // 部分返还
        partial_refund = 2,
    };
}
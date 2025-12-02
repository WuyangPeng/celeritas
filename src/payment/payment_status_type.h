#pragma once

namespace celeritas
{
    enum class payment_status_type
    {
        pending = 0, // 待处理
        paid = 1, // 已支付
        closed = 2, // 已关闭
        refunded = 3, // 已退款
    };
}
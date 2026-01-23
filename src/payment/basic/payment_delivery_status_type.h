#pragma once

namespace celeritas
{
    enum class payment_delivery_status_type
    {
        waiting_delivery = 0, // 等待交付
        delivered_success = 1, // 成功交付
        delivery_failed_need_repair = 2, // 交付失败需要修复
    };
}
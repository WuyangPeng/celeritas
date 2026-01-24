#pragma once

namespace celeritas
{
    enum class payment_status_type;
    enum class payment_delivery_status_type;
    enum class payment_platform_type;

    class sdk_payment_providers_key;
    class app_sdk_payment_providers;

    class order_create;
    class order_create_http_response;

    class recharge_notify;
    class we_chat_recharge_notify;
    class refund_notify;
    class we_chat_refund_notify;
}
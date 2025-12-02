#pragma once

#include "common/common_fwd.h"

namespace celeritas
{
    enum class payment_status_type;
    enum class payment_delivery_status_type;
    enum class payment_platform_type;

    class sdk_payment_providers_key;
    class app_sdk_payment_providers;
    class payment_service_base;

    class order_create_http_response;

    constexpr auto default_order_timeout = minute * 30;
}
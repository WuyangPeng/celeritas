#include "tencent_cloud_sms.h"

celeritas::tencent_cloud_sms::tencent_cloud_sms(sms_code sms_code, sms_providers sms_providers)
    : base_type{ std::move(sms_code), std::move(sms_providers) }
{
}

celeritas::send_sms_to_providers::void_awaitable_type celeritas::tencent_cloud_sms::execute()
{
    co_return;
}
#include "tencent_cloud_sms.h"

celeritas::tencent_cloud_sms::tencent_cloud_sms(const sms_code& sms_code, const sms_providers& sms_providers)
    : base_type{ sms_code, sms_providers }
{
}

celeritas::send_sms_to_providers::void_awaitable_type celeritas::tencent_cloud_sms::execute()
{
    co_return;
}
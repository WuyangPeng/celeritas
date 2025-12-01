#include "send_sms_to_providers.h"

#include <utility>
#include "send_sms_type.h"
#include "tencent_cloud_sms.h"
#include "common/celeritas_error.h"

celeritas::send_sms_to_providers::send_sms_to_providers(sms_code sms_code, sms_providers sms_providers)
    : sms_code_{ std::move(sms_code) }, sms_providers_{ std::move(sms_providers) }
{
}

celeritas::send_sms_to_providers::send_sms_to_providers_unique_ptr celeritas::send_sms_to_providers::create(sms_code sms_code, sms_providers sms_providers)
{
    const auto type = static_cast<send_sms_type>(sms_providers.get_process_type());
    switch (type)
    {
        case send_sms_type::tencent_cloud:
        {
            return std::make_unique<tencent_cloud_sms>(std::move(sms_code), std::move(sms_providers));
        }
        case send_sms_type::null:
        {
            break;
        }
    }

    throw celeritas_error{ "send sms type is error,type = {}", static_cast<int>(type) };
}
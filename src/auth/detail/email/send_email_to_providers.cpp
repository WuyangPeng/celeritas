#include "send_email_to_providers.h"
#include "send_email_type.h"
#include "tencent_cloud_email.h"
#include "common/core/celeritas_error.h"

celeritas::send_email_to_providers::send_email_to_providers(email_code email_code, email_providers email_providers)
    : email_code_{ std::move(email_code) }, email_providers_{ std::move(email_providers) }
{
}

celeritas::send_email_to_providers::send_email_to_providers_unique_ptr celeritas::send_email_to_providers::create(email_code email_code, email_providers email_providers)
{
    const auto type = static_cast<send_email_type>(email_providers.get_process_type());
    switch (type)
    {
        case send_email_type::tencent_cloud:
        {
            return std::make_unique<tencent_cloud_email>(std::move(email_code), std::move(email_providers));
        }
        case send_email_type::null:
        {
            break;
        }
    }

    throw celeritas_error{ "send email type is error,type = {}", static_cast<int>(type) };
}
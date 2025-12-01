#include "tencent_cloud_email.h"

celeritas::tencent_cloud_email::tencent_cloud_email(email_code email_code, email_providers email_providers)
    : base_type{ std::move(email_code), std::move(email_providers) }
{
}

celeritas::send_email_to_providers::void_awaitable_type celeritas::tencent_cloud_email::execute()
{
    co_return;
}
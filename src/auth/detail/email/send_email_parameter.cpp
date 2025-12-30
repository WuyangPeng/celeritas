#include "send_email_parameter.h"
#include "common/core_utilities/hmac_sha_256.h"

celeritas::send_email_parameter::send_email_parameter(const http_handle_parameter_shared_ptr& http_handle_parameter)
    : base_type{ http_handle_parameter }, response_{}
{
    init();
}

bool celeritas::send_email_parameter::is_failure() const
{
    return response_.has_value();
}

celeritas::send_email_response celeritas::send_email_parameter::get_response() const
{
    return response_.value();
}

void celeritas::send_email_parameter::init()
{
    if (const auto http_response = get_http_parameter())
    {
        response_ = send_email_response{ *http_response };

        return;
    }

    if (const auto hmac_sha256 = hmac_sha256::calculate_with_args(get_apps().get_app_secret(), get_app_id(), get_email(), get_timestamp());
        hmac_sha256 != get_sign())
    {
        response_ = send_email_response{ game_error_type::sign_error };
    }
}
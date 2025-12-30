#include "send_sms_parameter.h"
#include "auth/data/app_secret.h"
#include "common/core_utilities/hmac_sha_256.h"

celeritas::send_sms_parameter::send_sms_parameter(const http_handle_parameter_shared_ptr& http_handle_parameter)
    : base_type{ http_handle_parameter }, response_{}
{
    init();
}

bool celeritas::send_sms_parameter::is_failure() const
{
    return response_.has_value();
}

celeritas::send_sms_response celeritas::send_sms_parameter::get_response() const
{
    return response_.value();
}

void celeritas::send_sms_parameter::init()
{
    if (const auto http_response = get_http_parameter())
    {
        response_ = send_sms_response{ *http_response };

        return;
    }

    if (const auto hmac_sha256 = hmac_sha256::calculate_with_args(get_apps().get_app_secret(), get_app_id(), get_phone(), get_timestamp());
        hmac_sha256 != get_sign())
    {
        response_ = send_sms_response{ game_error_type::sign_error };
    }
}
#include "sdk_login_parameter.h"
#include "common/core/hmac_sha_256.h"

celeritas::sdk_login_parameter::sdk_login_parameter(const http_handle_parameter_shared_ptr& http_handle_parameter)
    : base_type{ http_handle_parameter }, response_{}
{
    init();
}

bool celeritas::sdk_login_parameter::is_failure() const
{
    return response_.has_value();
}

celeritas::sdk_login_response celeritas::sdk_login_parameter::get_response() const
{
    return *response_;
}

void celeritas::sdk_login_parameter::init()
{
    if (const auto http_response = get_http_parameter())
    {
        response_ = sdk_login_response{ *http_response };
    }

    if (const auto hmac_sha256 = hmac_sha256::calculate_with_args(get_apps().get_app_secret(), get_app_id(), get_sdk_token(), get_timestamp());
        hmac_sha256 != get_sign())
    {
        response_ = sdk_login_response{ game_error_type::sign_error };
    }
}
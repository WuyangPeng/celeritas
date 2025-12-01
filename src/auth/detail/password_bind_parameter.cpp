#include "auth_internal_fwd.h"
#include "password_bind_parameter.h"
#include "common/hmac_sha_256.h"

celeritas::password_bind_parameter::password_bind_parameter(const http_handle_parameter& http_handle_parameter)
    : base_type{ http_handle_parameter }, response_{}, token_{}
{
    init();
}

bool celeritas::password_bind_parameter::is_failure() const
{
    return response_.has_value();
}

celeritas::password_bind_response celeritas::password_bind_parameter::get_response() const
{
    return *response_;
}

std::string celeritas::password_bind_parameter::get_token() const
{
    return token_;
}

void celeritas::password_bind_parameter::init()
{
    if (const auto optional_token = get_param(token_describe.data());
        !optional_token)
    {
        response_ = password_bind_response{ game_error_type::invalid_parameter, "token is required" };

        return;
    }

    if (const auto http_response = get_http_parameter())
    {
        response_ = password_bind_response{ *http_response };
    }

    if (const auto hmac_sha256 = hmac_sha256::calculate_with_args(get_apps().get_app_secret(), get_app_id(), get_account(), get_password(), token_, get_timestamp());
        hmac_sha256 != get_sign())
    {
        response_ = password_bind_response{ game_error_type::sign_error };
    }
}
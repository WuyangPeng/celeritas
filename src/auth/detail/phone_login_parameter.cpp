#include "auth_internal_fwd.h"
#include "phone_login_parameter.h"
#include "common/hmac_sha_256.h"

#include <boost/lexical_cast.hpp>

celeritas::phone_login_parameter::phone_login_parameter(const http_handle_parameter& http_handle_parameter)
    : base_type{ http_handle_parameter }, response_{}, code_{}
{
    init();
}

bool celeritas::phone_login_parameter::is_failure() const
{
    return response_.has_value();
}

celeritas::phone_login_response celeritas::phone_login_parameter::get_response() const
{
    return *response_;
}

int celeritas::phone_login_parameter::get_code() const
{
    return code_;
}

void celeritas::phone_login_parameter::init()
{
    if (const auto http_response = get_http_parameter())
    {
        response_ = phone_login_response{ *http_response };
    }

    const auto optional_code = get_param(code_describe.data());
    if (!optional_code)
    {
        response_ = phone_login_response{ game_error_type::invalid_parameter, "code is required" };

        return;
    }

    code_ = boost::lexical_cast<int>(*optional_code);

    if (const auto hmac_sha256 = hmac_sha256::calculate_with_args(get_apps().get_app_secret(), get_app_id(), get_phone(), code_, get_timestamp());
        hmac_sha256 != get_sign())
    {
        response_ = phone_login_response{ game_error_type::sign_error };
    }
}
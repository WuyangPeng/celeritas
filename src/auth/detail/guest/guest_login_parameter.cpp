#include "guest_login_parameter.h"
#include "auth/data/app_secret.h"
#include "common/core/hmac_sha_256.h"
#include "database/generated/mysql/auth/account.h"

celeritas::guest_login_parameter::guest_login_parameter(const http_handle_parameter_shared_ptr& http_handle_parameter)
    : base_type{ http_handle_parameter }, response_{}, device_id_{}
{
    init();
}

std::string celeritas::guest_login_parameter::get_device_id() const
{
    return device_id_;
}

bool celeritas::guest_login_parameter::is_failure() const
{
    return response_.has_value();
}

celeritas::guest_login_response celeritas::guest_login_parameter::get_response() const
{
    return response_.value();
}

void celeritas::guest_login_parameter::init()
{
    const auto optional_device_id = get_param(account::device_id_describe.data());
    if (!optional_device_id)
    {
        response_ = guest_login_response{ game_error_type::invalid_parameter, "device id is required" };
        return;
    }

    device_id_ = *optional_device_id;

    if (const auto http_response = get_http_parameter())
    {
        response_ = guest_login_response{ *http_response };
        return;
    }

    const auto secret = app_secret::get_instance().get_key(get_app_id());

    if (const auto hmac_sha256 = hmac_sha256::calculate_with_args(secret, get_app_id(), device_id_, get_timestamp());
        hmac_sha256 != get_sign())
    {
        response_ = guest_login_response{ game_error_type::sign_error };
    }
}
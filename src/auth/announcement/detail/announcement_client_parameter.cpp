#include "announcement_client_parameter.h"
#include "auth/config/app_secret.h"
#include "common/core/hmac_sha_256.h"
#include "message/basic/game_error_type.h"

celeritas::announcement_client_parameter::announcement_client_parameter(
    const http_handle_parameter_shared_ptr& http_handle_parameter)
    : base_type{ http_handle_parameter }, response_{}
{
    init1();
}

bool celeritas::announcement_client_parameter::is_failure() const
{
    return response_.has_value();
}

celeritas::http_response celeritas::announcement_client_parameter::get_response() const
{
    return response_.value();
}

void celeritas::announcement_client_parameter::init1()
{
    if (const auto http_response = get_http_parameter())
    {
        response_ = *http_response;
        return;
    }

    const auto secret = app_secret::get_instance().get_key(get_app_id());

    if (const auto hmac_sha256 = hmac_sha256::calculate_with_args(secret, get_app_id(), get_timestamp());
        hmac_sha256 != get_sign())
    {
        response_ = http_response{ game_error_type::sign_error };
    }
}

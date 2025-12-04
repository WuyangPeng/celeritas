#include "auth_internal_fwd.h"
#include "login_servers_parameter.h"
#include "common/hmac_sha_256.h"
#include "database/generated/redis/auth/session_token.h"

celeritas::login_servers_parameter::login_servers_parameter(const http_handle_parameter& http_handle_parameter)
    : base_type{ http_handle_parameter }, response_{}, token_{}, region_{}, only_preferred_{}, include_details_{}, is_websocket_{}, apps_{}
{
    init();
}

bool celeritas::login_servers_parameter::is_failure() const
{
    return response_.has_value();
}

celeritas::login_servers_response celeritas::login_servers_parameter::get_response() const
{
    return *response_;
}

std::string celeritas::login_servers_parameter::get_token() const
{
    return token_;
}

celeritas::login_servers_parameter::optional_string celeritas::login_servers_parameter::get_region() const
{
    return region_;
}

bool celeritas::login_servers_parameter::is_only_preferred() const
{
    return only_preferred_;
}

bool celeritas::login_servers_parameter::is_include_details() const
{
    return include_details_;
}

bool celeritas::login_servers_parameter::is_is_websocket() const
{
    return is_websocket_;
}

celeritas::apps celeritas::login_servers_parameter::get_apps() const
{
    return *apps_;
}

void celeritas::login_servers_parameter::init()
{
    const auto optional_token = get_param(session_token::token_describe.data());
    if (!optional_token)
    {
        response_ = login_servers_response{ game_error_type::invalid_parameter, "token is required" };

        return;
    }

    token_ = *optional_token;

    region_ = get_param(region_describe.data());

    if (const auto optional_only_preferred = get_param(only_preferred_describe.data()))
    {
        only_preferred_ = *optional_only_preferred == "true";
    }

    if (const auto optional_include_details_describe = get_param(include_details_describe.data()))
    {
        include_details_ = *optional_include_details_describe == "true";
    }

    if (const auto optional_is_websocket_describe = get_param(is_websocket_describe.data()))
    {
        is_websocket_ = *optional_is_websocket_describe == "true";
    }

    if (const auto http_response = get_http_parameter())
    {
        response_ = login_servers_response{ *http_response };
    }

    if (const auto hmac_sha256 = hmac_sha256::calculate_with_args(get_apps().get_app_secret(), get_app_id(), token_, get_actual_region(), only_preferred_, include_details_, is_websocket_, get_timestamp());
        hmac_sha256 != get_sign())
    {
        response_ = login_servers_response{ game_error_type::sign_error };
    }
}

std::string celeritas::login_servers_parameter::get_actual_region() const
{
    if (region_)
    {
        return *region_;
    }

    return "";
}
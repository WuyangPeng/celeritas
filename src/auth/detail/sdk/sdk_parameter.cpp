#include "sdk_parameter.h"
#include "auth/data/app_secret.h"
#include "auth/detail/auth_internal_fwd.h"

#include <boost/lexical_cast.hpp>

celeritas::sdk_parameter::sdk_parameter(const http_handle_parameter& http_handle_parameter)
    : base_type{ http_handle_parameter }, sdk_token_{}, process_type_{}, apps_{}
{
}

std::string celeritas::sdk_parameter::get_sdk_token() const
{
    return sdk_token_;
}

celeritas::sdk_process_type celeritas::sdk_parameter::get_process_type() const
{
    return process_type_;
}

celeritas::apps celeritas::sdk_parameter::get_apps() const
{
    return *apps_;
}

celeritas::auth_parameter::optional_http_response celeritas::sdk_parameter::get_http_parameter()
{
    const auto optional_sdk_token = get_param(sdk_token_describe.data());
    if (!optional_sdk_token)
    {
        return http_response{ game_error_type::invalid_parameter, "sdk token is required" };
    }

    sdk_token_ = *optional_sdk_token;

    const auto optional_type = get_param(type_describe.data());
    if (!optional_type)
    {
        return http_response{ game_error_type::invalid_parameter, "type is required" };
    }

    process_type_ = static_cast<sdk_process_type>(boost::lexical_cast<int64_t>(*optional_type));

    if (const auto http_response = base_type::get_http_parameter())
    {
        return *http_response;
    }

    apps_ = app_secret::get_instance().get_apps(get_app_id());

    return std::nullopt;
}
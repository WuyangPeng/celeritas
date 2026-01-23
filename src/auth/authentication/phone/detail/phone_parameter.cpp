#include "phone_parameter.h"
#include "auth/auth_constant.h"
#include "auth/config/app_secret.h"
#include "common/core/hmac_sha_256.h"

#include <regex>

celeritas::phone_parameter::phone_parameter(const http_handle_parameter_shared_ptr& http_handle_parameter)
    : base_type{ http_handle_parameter }, phone_{}, apps_{}
{
}

std::string celeritas::phone_parameter::get_phone() const
{
    return phone_;
}

celeritas::apps celeritas::phone_parameter::get_apps() const
{
    return *apps_;
}

celeritas::auth_parameter::optional_http_response celeritas::phone_parameter::get_http_parameter()
{
    const auto optional_phone = get_param(phone_describe.data());
    if (!optional_phone)
    {
        return http_response{ game_error_type::invalid_parameter, "phone is required" };
    }

    phone_ = *optional_phone;

    if (const std::regex phone_regex{ R"(^1\d{10}$)" };
        !std::regex_match(phone_, phone_regex))
    {
        return http_response{ game_error_type::invalid_parameter, "phone is invalid" };
    }

    if (const auto http_response = base_type::get_http_parameter())
    {
        return *http_response;
    }

    apps_ = app_secret::get_instance().get_apps(get_app_id());

    return std::nullopt;
}
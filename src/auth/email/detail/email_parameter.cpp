#include "email_parameter.h"
#include "auth/auth_constant.h"
#include "auth/config/app_secret.h"

#include <regex>

celeritas::email_parameter::email_parameter(const http_handle_parameter_shared_ptr& http_handle_parameter)
    : base_type{ http_handle_parameter }
{
}

std::string celeritas::email_parameter::get_email() const
{
    return email_;
}

celeritas::apps celeritas::email_parameter::get_apps() const
{
    return *apps_;
}

celeritas::auth_parameter::optional_http_response celeritas::email_parameter::get_http_parameter()
{
    const auto optional_email = get_param(email_describe.data());
    if (!optional_email)
    {
        return http_response{ game_error_type::invalid_parameter, "email is required" };
    }

    email_ = *optional_email;

    if (const std::regex email_regex{ R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)" };
        !std::regex_match(email_, email_regex))
    {
        return http_response{ game_error_type::invalid_parameter, "email is invalid" };
    }

    if (const auto http_response = base_type::get_http_parameter())
    {
        return *http_response;
    }

    apps_ = *app_secret::get_instance().get_apps(get_app_id());

    return std::nullopt;
}
#include "password_parameter.h"
#include "auth/auth_constant.h"
#include "auth/config/app_secret.h"

#include <regex>

celeritas::password_parameter::password_parameter(const http_handle_parameter_shared_ptr& http_handle_parameter)
    : base_type{ http_handle_parameter }, account_{}, password_{}
{
}

std::string celeritas::password_parameter::get_account() const
{
    return account_;
}

std::string celeritas::password_parameter::get_password() const
{
    return password_;
}

celeritas::apps celeritas::password_parameter::get_apps() const
{
    return *apps_;
}

celeritas::auth_parameter::optional_http_response celeritas::password_parameter::get_http_parameter()
{
    const auto optional_account = get_param(account_describe.data());
    if (!optional_account)
    {
        return http_response{ game_error_type::invalid_parameter, "account is required" };
    }

    account_ = *optional_account;

    if (const std::regex account_regex{ R"(^[a-zA-Z0-9_.-]{4,32}$)" };
        !std::regex_match(account_, account_regex))
    {
        return http_response{ game_error_type::invalid_parameter, "account is invalid" };
    }

    const auto optional_password = get_param("password");
    if (!optional_password)
    {
        return http_response{ game_error_type::invalid_parameter, "password is required" };
    }

    password_ = *optional_password;

    if (const std::regex password_regex{ R"(^[a-zA-Z0-9!@#$%^&*()_+-=]{8,32}$)" };
        !std::regex_match(password_, password_regex))
    {
        return http_response{ game_error_type::invalid_parameter, "password is invalid" };
    }

    if (const auto http_response = base_type::get_http_parameter())
    {
        return *http_response;
    }

    apps_ = app_secret::get_instance().get_apps(get_app_id());

    return std::nullopt;
}
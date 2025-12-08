#include "password_login_response.h"

celeritas::password_login_response::password_login_response(const game_error_type code)
    : bass_type{ code }
{
}

celeritas::password_login_response::password_login_response(const game_error_type code, std::string message)
    : bass_type{ code, std::move(message) }
{
}

celeritas::password_login_response::password_login_response(const game_error_type code, std::string message, std::string token, const int64_t expire_milliseconds)
    : bass_type{ code, std::move(message), std::move(token), expire_milliseconds }
{
}

celeritas::password_login_response::password_login_response(bass_type token_http_response)
    : bass_type{ std::move(token_http_response) }
{
}

celeritas::password_login_response::password_login_response(http_response http_response)
    : bass_type{ std::move(http_response) }
{
}

celeritas::password_login_response
    celeritas::password_login_response::from_json_string(const std::string& json_string)
{
    auto token_http_response = bass_type::from_json_string(json_string);

    return password_login_response{ std::move(token_http_response) };
}

celeritas::password_login_response celeritas::tag_invoke(password_login_response_tag, const http_response::json_value& value)
{
    return password_login_response{ tag_invoke(token_http_response_tag{}, value) };
}

void celeritas::tag_invoke(const boost::json::value_from_tag tag, password_login_response::json_value& value, const password_login_response& password_login_response)
{
    tag_invoke(tag, value, password_login_response::bass_type{ password_login_response.get_code(), password_login_response.get_message(), password_login_response.get_token(), password_login_response.get_expire_milliseconds() });
}
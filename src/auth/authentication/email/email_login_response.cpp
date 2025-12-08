#include "email_login_response.h"

celeritas::email_login_response::email_login_response(const game_error_type code)
    : bass_type{ code }
{
}

celeritas::email_login_response::email_login_response(const game_error_type code, std::string message)
    : bass_type{ code, std::move(message) }
{
}

celeritas::email_login_response::email_login_response(const game_error_type code, std::string message,
                                                      std::string token, const int64_t expire_milliseconds)
    : bass_type{ code, std::move(message), std::move(token), expire_milliseconds }
{
}

celeritas::email_login_response::email_login_response(bass_type token_http_response)
    : bass_type{ std::move(token_http_response) }
{
}

celeritas::email_login_response::email_login_response(http_response http_response)
    : bass_type{ std::move(http_response) }
{
}

celeritas::email_login_response celeritas::email_login_response::from_json_string(const std::string& json_string)
{
    auto token_http_response = bass_type::from_json_string(json_string);

    return email_login_response{ std::move(token_http_response) };
}

celeritas::email_login_response
    celeritas::tag_invoke(email_login_response_tag, const http_response::json_value& value)
{
    return email_login_response{ tag_invoke(token_http_response_tag{}, value) };
}

void celeritas::tag_invoke(const boost::json::value_from_tag tag, email_login_response::json_value& value,
                           const email_login_response& email_login_response)
{
    tag_invoke(tag, value, email_login_response::bass_type{
                   email_login_response.get_code(), email_login_response.get_message(),
                   email_login_response.get_token(), email_login_response.get_expire_milliseconds()
               });
}
#include "phone_login_response.h"

celeritas::phone_login_response::phone_login_response(const game_error_type code)
    : bass_type{ code }
{
}

celeritas::phone_login_response::phone_login_response(const game_error_type code, std::string message)
    : bass_type{ code, std::move(message) }
{
}

celeritas::phone_login_response::phone_login_response(const game_error_type code, std::string message, std::string token, const int64_t expire_milliseconds)
    : bass_type{ code, std::move(message), std::move(token), expire_milliseconds }
{
}

celeritas::phone_login_response::phone_login_response(bass_type token_http_response)
    : bass_type{ std::move(token_http_response) }
{
}

celeritas::phone_login_response::phone_login_response(http_response http_response)
    : bass_type{ std::move(http_response) }
{
}

celeritas::phone_login_response celeritas::phone_login_response::from_json_string(const std::string& json_string)
{
    auto token_http_response = bass_type::from_json_string(json_string);

    return phone_login_response{ std::move(token_http_response) };
}

celeritas::phone_login_response celeritas::tag_invoke(phone_login_response_tag, const http_response::json_value& value)
{
    return phone_login_response{ tag_invoke(token_http_response_tag{}, value) };
}

void celeritas::tag_invoke(const boost::json::value_from_tag tag, phone_login_response::json_value& value, const phone_login_response& phone_login_response)
{
    tag_invoke(tag, value, phone_login_response::bass_type{ phone_login_response.get_code(), phone_login_response.get_message(), phone_login_response.get_token(), phone_login_response.get_expire_milliseconds() });
}
#include "sdk_login_response.h"

celeritas::sdk_login_response::sdk_login_response(const game_error_type code)
    : bass_type{ code }
{
}

celeritas::sdk_login_response::sdk_login_response(const game_error_type code, std::string message)
    : bass_type{ code, std::move(message) }
{
}

celeritas::sdk_login_response::sdk_login_response(const game_error_type code, std::string message, std::string token, const int64_t expire_milliseconds)
    : bass_type{ code, std::move(message), std::move(token), expire_milliseconds }
{
}

celeritas::sdk_login_response::sdk_login_response(bass_type token_http_response)
    : bass_type{ std::move(token_http_response) }
{
}

celeritas::sdk_login_response::sdk_login_response(http_response http_response)
    : bass_type{ std::move(http_response) }
{
}

celeritas::sdk_login_response celeritas::sdk_login_response::from_json_string(const std::string& json_string)
{
    auto token_http_response = bass_type::from_json_string(json_string);

    return sdk_login_response{ std::move(token_http_response) };
}

celeritas::sdk_login_response celeritas::tag_invoke(sdk_login_response_tag, const http_response::json_value& value)
{
    return sdk_login_response{ tag_invoke(token_http_response_tag{}, value) };
}

void celeritas::tag_invoke(const boost::json::value_from_tag tag, sdk_login_response::json_value& value, const sdk_login_response& sdk_login_response)
{
    tag_invoke(tag, value, sdk_login_response::bass_type{ sdk_login_response.get_code(), sdk_login_response.get_message(), sdk_login_response.get_token(), sdk_login_response.get_expire_milliseconds() });
}
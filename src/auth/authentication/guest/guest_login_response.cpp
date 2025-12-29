#include "guest_login_response.h"
#include "../../../common/core_utilities/celeritas_error.h"
#include "common/logger.h"

celeritas::guest_login_response::guest_login_response(const game_error_type code)
    : bass_type{ code }
{
    LOG_CHANNEL(auth_channel, trace) << "login error code:" << get_game_error_description(code) << ",message:" << get_message();
}

celeritas::guest_login_response::guest_login_response(const game_error_type code, std::string message)
    : bass_type{ code, std::move(message) }
{
    LOG_CHANNEL(auth_channel, trace) << "login error code:" << get_game_error_description(code) << ",message:" << get_message();
}

celeritas::guest_login_response::guest_login_response(const game_error_type code, std::string message, std::string token, const int64_t expire_milliseconds)
    : bass_type{ code, std::move(message), std::move(token), expire_milliseconds }
{
    LOG_CHANNEL(auth_channel, trace) << "login error code:" << get_game_error_description(code) << ",message:" << get_message();
}

celeritas::guest_login_response::guest_login_response(bass_type token_http_response)
    : bass_type{ std::move(token_http_response) }
{
    LOG_CHANNEL(auth_channel, trace) << "login error code:" << get_game_error_description(get_code()) << ",message:" << get_message();
}

celeritas::guest_login_response::guest_login_response(http_response http_response)
    : bass_type{ std::move(http_response) }
{
    LOG_CHANNEL(auth_channel, trace) << "login error code:" << get_game_error_description(get_code()) << ",message:" << get_message();
}

celeritas::guest_login_response celeritas::guest_login_response::from_json_string(const std::string& json_string)
{
    auto token_http_response = bass_type::from_json_string(json_string);

    return guest_login_response{ std::move(token_http_response) };
}

celeritas::guest_login_response celeritas::tag_invoke(guest_login_response_tag, const guest_login_response::json_value& value)
{
    return guest_login_response{ tag_invoke(token_http_response_tag{}, value) };
}

void celeritas::tag_invoke(const boost::json::value_from_tag tag, guest_login_response::json_value& value, const guest_login_response& guest_login_response)
{
    tag_invoke(tag, value, guest_login_response::bass_type{ guest_login_response.get_code(), guest_login_response.get_message(), guest_login_response.get_token(), guest_login_response.get_expire_milliseconds() });
}

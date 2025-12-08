#include "send_email_response.h"

celeritas::send_email_response::send_email_response(const game_error_type code)
    : bass_type{ code }
{
}

celeritas::send_email_response::send_email_response(const game_error_type code, std::string message)
    : bass_type{ code, std::move(message) }
{
}

celeritas::send_email_response::send_email_response(bass_type http_response)
    : bass_type{ std::move(http_response) }
{
}

celeritas::send_email_response celeritas::send_email_response::from_json_string(const std::string& json_string)
{
    auto http_response = bass_type::from_json_string(json_string);

    return send_email_response{ std::move(http_response) };
}

celeritas::send_email_response celeritas::tag_invoke(send_email_response_tag, const http_response::json_value& value)
{
    return send_email_response{ tag_invoke(http_response_tag{}, value) };
}

void celeritas::tag_invoke(const boost::json::value_from_tag tag, send_email_response::json_value& value, const send_email_response& send_email_response)
{
    tag_invoke(tag, value, send_email_response::bass_type{ send_email_response.get_code(), send_email_response.get_message() });
}
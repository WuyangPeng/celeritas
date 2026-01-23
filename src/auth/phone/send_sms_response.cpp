#include "send_sms_response.h"

celeritas::send_sms_response::send_sms_response(const game_error_type code)
    : bass_type{ code }
{
}

celeritas::send_sms_response::send_sms_response(const game_error_type code, std::string message)
    : bass_type{ code, std::move(message) }
{
}

celeritas::send_sms_response::send_sms_response(bass_type http_response)
    : bass_type{ std::move(http_response) }
{
}

celeritas::send_sms_response celeritas::send_sms_response::from_json_string(const std::string& json_string)
{
    auto http_response = bass_type::from_json_string(json_string);

    return send_sms_response{ std::move(http_response) };
}

celeritas::send_sms_response celeritas::tag_invoke(send_sms_response_tag, const http_response::json_value& value)
{
    return send_sms_response{ tag_invoke(http_response_tag{}, value) };
}

void celeritas::tag_invoke(const boost::json::value_from_tag tag, send_sms_response::json_value& value, const send_sms_response& send_sms_response)
{
    tag_invoke(tag, value, send_sms_response::bass_type{ send_sms_response.get_code(), send_sms_response.get_message() });
}

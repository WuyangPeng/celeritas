#include "email_bind_response.h"
#include "common/celeritas_error.h"

celeritas::email_bind_response::email_bind_response(const game_error_type code, std::string message)
    : bass_type{ code, std::move(message) }
{
}

celeritas::email_bind_response::email_bind_response(bass_type http_response)
    : bass_type{ std::move(http_response) }
{
}

celeritas::email_bind_response celeritas::email_bind_response::from_json_string(const std::string& json_string)
{
    auto http_response = bass_type::from_json_string(json_string);

    return email_bind_response{ std::move(http_response) };
}

celeritas::email_bind_response celeritas::tag_invoke(email_bind_response_tag, const http_response::json_value& value)
{
    return email_bind_response{ tag_invoke(http_response_tag{}, value) };
}

void celeritas::tag_invoke(const boost::json::value_from_tag tag, email_bind_response::json_value& value, const email_bind_response& email_bind_response)
{
    tag_invoke(tag, value, email_bind_response::bass_type{ email_bind_response.get_code(), email_bind_response.get_message() });
}
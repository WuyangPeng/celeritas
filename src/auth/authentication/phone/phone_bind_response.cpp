#include "phone_bind_response.h"

celeritas::phone_bind_response::phone_bind_response(const game_error_type code)
    : bass_type{ code }
{
}

celeritas::phone_bind_response::phone_bind_response(const game_error_type code, std::string message)
    : bass_type{ code, std::move(message) }
{
}

celeritas::phone_bind_response::phone_bind_response(bass_type http_response)
    : bass_type{ std::move(http_response) }
{
}

celeritas::phone_bind_response celeritas::phone_bind_response::from_json_string(const std::string& json_string)
{
    auto http_response = bass_type::from_json_string(json_string);

    return phone_bind_response{ std::move(http_response) };
}

celeritas::phone_bind_response celeritas::tag_invoke(phone_bind_response_tag, const http_response::json_value& value)
{
    return phone_bind_response{ tag_invoke(http_response_tag{}, value) };
}

void celeritas::tag_invoke(const boost::json::value_from_tag tag, phone_bind_response::json_value& value,
                           const phone_bind_response& phone_bind_response)
{
    tag_invoke(tag, value, phone_bind_response::bass_type{
                   phone_bind_response.get_code(), phone_bind_response.get_message()
               });
}
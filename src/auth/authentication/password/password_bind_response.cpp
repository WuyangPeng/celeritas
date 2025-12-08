#include "password_bind_response.h"

celeritas::password_bind_response::password_bind_response(const game_error_type code)
    : bass_type{ code }
{
}

celeritas::password_bind_response::password_bind_response(const game_error_type code, std::string message)
    : bass_type{ code, std::move(message) }
{
}

celeritas::password_bind_response::password_bind_response(bass_type http_response)
    : bass_type{ std::move(http_response) }
{
}

celeritas::password_bind_response celeritas::password_bind_response::from_json_string(const std::string& json_string)
{
    auto http_response = bass_type::from_json_string(json_string);

    return password_bind_response{ std::move(http_response) };
}

celeritas::password_bind_response celeritas::tag_invoke(password_bind_response_tag,
                                                        const http_response::json_value& value)
{
    return password_bind_response{ tag_invoke(http_response_tag{}, value) };
}

void celeritas::tag_invoke(const boost::json::value_from_tag tag, password_bind_response::json_value& value,
                           const password_bind_response& password_bind_response)
{
    tag_invoke(tag, value, password_bind_response::bass_type{
                   password_bind_response.get_code(), password_bind_response.get_message()
               });
}
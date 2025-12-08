#include "sdk_bind_response.h"

celeritas::sdk_bind_response::sdk_bind_response(const game_error_type code)
    : bass_type{ code }
{
}

celeritas::sdk_bind_response::sdk_bind_response(const game_error_type code, std::string message)
    : bass_type{ code, std::move(message) }
{
}

celeritas::sdk_bind_response::sdk_bind_response(bass_type http_response)
    : bass_type{ std::move(http_response) }
{
}

celeritas::sdk_bind_response celeritas::sdk_bind_response::from_json_string(const std::string& json_string)
{
    auto http_response = bass_type::from_json_string(json_string);

    return sdk_bind_response{ std::move(http_response) };
}

celeritas::sdk_bind_response celeritas::tag_invoke(sdk_bind_response_tag, const http_response::json_value& value)
{
    return sdk_bind_response{ tag_invoke(http_response_tag{}, value) };
}

void celeritas::tag_invoke(const boost::json::value_from_tag tag, sdk_bind_response::json_value& value,
                           const sdk_bind_response& sdk_bind_response)
{
    tag_invoke(tag, value, sdk_bind_response::bass_type{ sdk_bind_response.get_code(), sdk_bind_response.get_message() });
}
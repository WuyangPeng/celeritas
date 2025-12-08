#pragma once

#include "message/http_response.h"

namespace celeritas
{
    class sdk_bind_response final : public http_response
    {
    public:
        using class_type = sdk_bind_response;
        using bass_type = http_response;

        explicit sdk_bind_response(game_error_type code);

        sdk_bind_response(game_error_type code, std::string message);

        explicit sdk_bind_response(bass_type http_response);

        [[nodiscard]] static sdk_bind_response from_json_string(const std::string& json_string);
    };

    using sdk_bind_response_tag = boost::json::value_to_tag<sdk_bind_response>;

    [[nodiscard]] sdk_bind_response tag_invoke(sdk_bind_response_tag, const http_response::json_value& value);

    void tag_invoke(boost::json::value_from_tag tag, sdk_bind_response::json_value& value, const sdk_bind_response& sdk_bind_response);
}
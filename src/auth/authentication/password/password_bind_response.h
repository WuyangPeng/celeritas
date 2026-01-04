#pragma once

#include "message/basic/http_response.h"

namespace celeritas
{
    class password_bind_response final : public http_response
    {
    public:
        using class_type = password_bind_response;
        using bass_type = http_response;

        explicit password_bind_response(game_error_type code);

        password_bind_response(game_error_type code, std::string message);

        explicit password_bind_response(bass_type http_response);

        [[nodiscard]] static password_bind_response from_json_string(const std::string& json_string);
    };

    using password_bind_response_tag = boost::json::value_to_tag<password_bind_response>;

    [[nodiscard]] password_bind_response tag_invoke(password_bind_response_tag, const http_response::json_value& value);

    void tag_invoke(boost::json::value_from_tag tag, password_bind_response::json_value& value, const password_bind_response& password_bind_response);
}
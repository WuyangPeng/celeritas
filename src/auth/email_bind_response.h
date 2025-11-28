#pragma once

#include "message/http_response.h"

namespace celeritas
{
    class email_bind_response final : public http_response
    {
    public:
        using class_type = email_bind_response;
        using bass_type = http_response;

        email_bind_response(game_error_type code, std::string message);

        explicit email_bind_response(bass_type http_response);

        [[nodiscard]] static email_bind_response from_json_string(const std::string& json_string);
    };

    using email_bind_response_tag = boost::json::value_to_tag<email_bind_response>;

    [[nodiscard]] email_bind_response tag_invoke(email_bind_response_tag, const http_response::json_value& value);

    void tag_invoke(boost::json::value_from_tag tag, email_bind_response::json_value& value, const email_bind_response& email_bind_response);
}
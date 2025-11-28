#pragma once

#include "message/http_response.h"

namespace celeritas
{
    class phone_bind_response final : public http_response
    {
    public:
        using class_type = phone_bind_response;
        using bass_type = http_response;

        explicit phone_bind_response(game_error_type code);

        phone_bind_response(game_error_type code, std::string message);

        explicit phone_bind_response(bass_type http_response);

        [[nodiscard]] static phone_bind_response from_json_string(const std::string& json_string);
    };

    using phone_bind_response_tag = boost::json::value_to_tag<phone_bind_response>;

    [[nodiscard]] phone_bind_response tag_invoke(phone_bind_response_tag, const http_response::json_value& value);

    void tag_invoke(boost::json::value_from_tag tag, phone_bind_response::json_value& value, const phone_bind_response& phone_bind_response);
}
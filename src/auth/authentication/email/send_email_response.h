#pragma once

#include "message/basic/http_response.h"

namespace celeritas
{
    class send_email_response final : public http_response
    {
    public:
        using class_type = send_email_response;
        using bass_type = http_response;

        explicit send_email_response(game_error_type code);

        send_email_response(game_error_type code, std::string message);

        explicit send_email_response(bass_type http_response);

        [[nodiscard]] static send_email_response from_json_string(const std::string& json_string);
    };

    using send_email_response_tag = boost::json::value_to_tag<send_email_response>;

    [[nodiscard]] send_email_response tag_invoke(send_email_response_tag, const http_response::json_value& value);

    void tag_invoke(boost::json::value_from_tag tag, send_email_response::json_value& value, const send_email_response& send_email_response);
}

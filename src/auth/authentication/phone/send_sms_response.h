#pragma once

#include "message/http_response.h"

namespace celeritas
{
    class send_sms_response final : public http_response
    {
    public:
        using class_type = send_sms_response;
        using bass_type = http_response;

        explicit send_sms_response(game_error_type code);

        send_sms_response(game_error_type code, std::string message);

        explicit send_sms_response(bass_type http_response);

        [[nodiscard]] static send_sms_response from_json_string(const std::string& json_string);
    };

    using send_sms_response_tag = boost::json::value_to_tag<send_sms_response>;

    [[nodiscard]] send_sms_response tag_invoke(send_sms_response_tag, const http_response::json_value& value);

    void tag_invoke(boost::json::value_from_tag tag, send_sms_response::json_value& value, const send_sms_response& send_sms_response);
}
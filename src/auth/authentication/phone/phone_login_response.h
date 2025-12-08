#pragma once

#include "auth/core/token_http_response.h"

namespace celeritas
{
    class phone_login_response final : public token_http_response
    {
    public:
        using class_type = phone_login_response;
        using bass_type = token_http_response;

        explicit phone_login_response(game_error_type code);

        phone_login_response(game_error_type code, std::string message);

        phone_login_response(game_error_type code, std::string message, std::string token, int64_t expire_milliseconds);

        explicit phone_login_response(bass_type token_http_response);

        explicit phone_login_response(http_response http_response);

        [[nodiscard]] static phone_login_response from_json_string(const std::string& json_string);
    };

    using phone_login_response_tag = boost::json::value_to_tag<phone_login_response>;

    [[nodiscard]] phone_login_response tag_invoke(phone_login_response_tag, const http_response::json_value& value);

    void tag_invoke(boost::json::value_from_tag tag, phone_login_response::json_value& value,
                    const phone_login_response& phone_login_response);
}
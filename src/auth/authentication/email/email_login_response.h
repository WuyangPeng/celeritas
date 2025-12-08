#pragma once

#include "auth/core/token_http_response.h"

namespace celeritas {
    class email_login_response final : public token_http_response {
    public:
        using class_type = email_login_response;
        using bass_type = token_http_response;

        explicit email_login_response(game_error_type code);

        email_login_response(game_error_type code, std::string message);

        email_login_response(game_error_type code, std::string message, std::string token, int64_t expire_milliseconds);

        explicit email_login_response(bass_type token_http_response);

        explicit email_login_response(http_response http_response);

        [[nodiscard]] static email_login_response from_json_string(const std::string &json_string);
    };

    using email_login_response_tag = boost::json::value_to_tag<email_login_response>;

    [[nodiscard]] email_login_response tag_invoke(email_login_response_tag, const http_response::json_value &value);

    void tag_invoke(boost::json::value_from_tag tag, email_login_response::json_value &value,
                    const email_login_response &email_login_response);
}
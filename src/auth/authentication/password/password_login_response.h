#pragma once

#include "auth/core/token_http_response.h"

namespace celeritas {
    class password_login_response final : public token_http_response {
    public:
        using class_type = password_login_response;
        using bass_type = token_http_response;

        explicit password_login_response(game_error_type code);

        password_login_response(game_error_type code, std::string message);

        password_login_response(game_error_type code, std::string message, std::string token,
                                int64_t expire_milliseconds);

        explicit password_login_response(bass_type token_http_response);

        explicit password_login_response(http_response http_response);

        [[nodiscard]] static password_login_response from_json_string(const std::string &json_string);
    };

    using password_login_response_tag = boost::json::value_to_tag<password_login_response>;

    [[nodiscard]] password_login_response tag_invoke(password_login_response_tag,
                                                     const http_response::json_value &value);

    void tag_invoke(boost::json::value_from_tag tag, password_login_response::json_value &value,
                    const password_login_response &password_login_response);
}
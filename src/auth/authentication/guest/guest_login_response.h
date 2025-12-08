#pragma once

#include "auth/core/token_http_response.h"

namespace celeritas
{
    class guest_login_response final : public token_http_response
    {
    public:
        using class_type = guest_login_response;
        using bass_type = token_http_response;

        guest_login_response() noexcept = default;

        explicit guest_login_response(game_error_type code);

        guest_login_response(game_error_type code, std::string message);

        guest_login_response(game_error_type code, std::string message, std::string token, int64_t expire_milliseconds);

        explicit guest_login_response(bass_type token_http_response);

        explicit guest_login_response(http_response http_response);

        [[nodiscard]] static guest_login_response from_json_string(const std::string& json_string);
    };

    using guest_login_response_tag = boost::json::value_to_tag<guest_login_response>;

    [[nodiscard]] guest_login_response tag_invoke(guest_login_response_tag, const guest_login_response::json_value& value);

    void tag_invoke(boost::json::value_from_tag tag, guest_login_response::json_value& value, const guest_login_response& guest_login_response);
}
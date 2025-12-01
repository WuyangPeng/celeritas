#pragma once

#include "token_http_response.h"

namespace celeritas
{
    class sdk_login_response final : public token_http_response
    {
    public:
        using class_type = sdk_login_response;
        using bass_type = token_http_response;

        explicit sdk_login_response(game_error_type code);

        sdk_login_response(game_error_type code, std::string message);

        sdk_login_response(game_error_type code, std::string message, std::string token, int64_t expire_milliseconds);

        explicit sdk_login_response(bass_type token_http_response);

        explicit sdk_login_response(http_response http_response);

        [[nodiscard]] static sdk_login_response from_json_string(const std::string& json_string);
    };

    using sdk_login_response_tag = boost::json::value_to_tag<sdk_login_response>;

    [[nodiscard]] sdk_login_response tag_invoke(sdk_login_response_tag, const http_response::json_value& value);

    void tag_invoke(boost::json::value_from_tag tag, sdk_login_response::json_value& value, const sdk_login_response& sdk_login_response);
}
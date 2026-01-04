#pragma once

#include "message/basic/http_response.h"

namespace celeritas
{
    class token_http_response : public http_response
    {
    public:
        using class_type = token_http_response;
        using bass_type = http_response;

        token_http_response() noexcept = default;

        explicit token_http_response(game_error_type code);

        token_http_response(game_error_type code, std::string message);

        token_http_response(game_error_type code, std::string message, std::string token, int64_t expire_milliseconds);

        token_http_response(bass_type http_response, std::string token, int64_t expire_milliseconds);

        explicit token_http_response(bass_type http_response);

        [[nodiscard]] std::string get_token() const;

        void set_token(const std::string& token);

        [[nodiscard]] int64_t get_expire_milliseconds() const;

        void set_expire_milliseconds(int64_t expireMilliseconds);

        [[nodiscard]] std::string to_json_string() const override;

        [[nodiscard]] static token_http_response from_json_string(const std::string& json_string);

        [[nodiscard]] static token_http_response tag_invoke(const json_value& value);

        static constexpr std::string_view token_description = "token";
        static constexpr std::string_view expire_milliseconds_description = "expire_milliseconds";

    private:
        [[nodiscard]] static token_http_response do_from_json_string(const std::string& json_string);

        std::string token_;
        int64_t expire_milliseconds_ = 0;
    };

    using token_http_response_tag = boost::json::value_to_tag<token_http_response>;

    [[nodiscard]] token_http_response tag_invoke(token_http_response_tag, const http_response::json_value& value);

    void tag_invoke(boost::json::value_from_tag, token_http_response::json_value& value, const token_http_response& token_http_response);
}
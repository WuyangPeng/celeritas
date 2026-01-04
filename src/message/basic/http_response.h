#pragma once

#include "game_error_type.h"

#include <boost/json.hpp>

#include <string>

namespace celeritas
{
    class http_response
    {
    public:
        using class_type = http_response;
        using json_value = boost::json::value;

        http_response() noexcept = default;

        explicit http_response(game_error_type code);

        http_response(game_error_type code, std::string message);

        virtual ~http_response() noexcept = default;

        http_response(const http_response& rhs) = default;

        http_response& operator=(const http_response& rhs) = default;

        http_response(http_response&& rhs) noexcept = default;

        http_response& operator=(http_response&& rhs) noexcept = default;

        [[nodiscard]] game_error_type get_code() const;

        void set_code(game_error_type code);

        [[nodiscard]] std::string get_message() const;

        void set_message(const std::string& message);

        [[nodiscard]] virtual std::string to_json_string() const;

        [[nodiscard]] static http_response from_json_string(const std::string& json_string);

        [[nodiscard]] static http_response tag_invoke(const json_value& value);

        static constexpr std::string_view code_description = "code";
        static constexpr std::string_view message_description = "message";

    private:
        [[nodiscard]] static http_response do_from_json_string(const std::string& json_string);

        game_error_type code_ = game_error_type::unknown;
        std::string message_;
    };

    using http_response_tag = boost::json::value_to_tag<http_response>;

    [[nodiscard]] http_response tag_invoke(http_response_tag, const http_response::json_value& value);

    void tag_invoke(boost::json::value_from_tag, http_response::json_value& value, const http_response& http_response);
}
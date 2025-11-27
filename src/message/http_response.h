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

        http_response() noexcept = default;

        http_response(game_error_type code, std::string message);

        [[nodiscard]] std::string to_json_string() const;

        [[nodiscard]] static http_response from_json_string(const std::string& json_string);

        static constexpr std::string_view code_description = "code";
        static constexpr std::string_view message_description = "message";

        [[nodiscard]] game_error_type get_code() const;

        void set_code(game_error_type code);

        [[nodiscard]] std::string get_message() const;

        void set_message(const std::string& message);

    private:
        game_error_type code_ = game_error_type::unknown;
        std::string message_;
    };

    [[nodiscard]] http_response tag_invoke(boost::json::value_to_tag<http_response>, const boost::json::value& value);

    void tag_invoke(boost::json::value_from_tag, boost::json::value& value, const http_response& http_response);
}
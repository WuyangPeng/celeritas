#pragma once

#include "message/http_response.h"

namespace celeritas
{
    class order_create_http_response final : public http_response
    {
    public:
        using class_type = order_create_http_response;
        using bass_type = http_response;

        order_create_http_response() noexcept = default;

        explicit order_create_http_response(game_error_type code);

        order_create_http_response(game_error_type code, std::string message);

        order_create_http_response(game_error_type code, std::string message, std::string order_id, std::string platform, std::string payment_params_json, int amount);

        order_create_http_response(bass_type http_response, std::string order_id, std::string platform, std::string payment_params_json, int amount);

        explicit order_create_http_response(bass_type http_response);

        [[nodiscard]] std::string to_json_string() const override;

        [[nodiscard]] static order_create_http_response from_json_string(const std::string& json_string);

        [[nodiscard]] static order_create_http_response tag_invoke(const json_value& value);

        [[nodiscard]] std::string get_order_id() const;

        [[nodiscard]] std::string get_platform() const;

        [[nodiscard]] std::string get_payment_params_json() const;

        [[nodiscard]] int get_amount() const;

        static constexpr std::string_view order_id_description = "order_id";
        static constexpr std::string_view platform_description = "platform";
        static constexpr std::string_view payment_params_json_description = "payment_params_json";
        static constexpr std::string_view amount_description = "amount";

    private:
        [[nodiscard]] static order_create_http_response do_from_json_string(const std::string& json_string);

        std::string order_id_;
        std::string platform_;
        std::string payment_params_json_;
        int amount_;
    };

    using order_create_http_response_tag = boost::json::value_to_tag<order_create_http_response>;

    [[nodiscard]] order_create_http_response tag_invoke(order_create_http_response_tag, const http_response::json_value& value);

    void tag_invoke(boost::json::value_from_tag, order_create_http_response::json_value& value, const order_create_http_response& order_create_http_response);
}
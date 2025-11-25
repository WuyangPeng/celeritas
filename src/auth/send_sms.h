#pragma once

#include "message/http_handle_parameter.h"

namespace celeritas
{
    class send_sms
    {
    public:
        using class_type = send_sms;
        using void_awaitable_type = boost::asio::awaitable<void>;

        explicit send_sms(http_handle_parameter handle_parameter);

        [[nodiscard]] void_awaitable_type response();

    private:
        [[nodiscard]] static std::string calculate_hmac_sha256(int app_id, const std::string& phone, int64_t timestamp, const std::string& secret_key);

        void send_sdk_sms(const std::string& phone, int code);

        http_handle_parameter handle_parameter_;
    };
}
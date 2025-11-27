#pragma once

#include "database/generated/redis/auth/email_code.h"
#include "message/http_handle_parameter.h"

namespace celeritas
{
    class send_email
    {
    public:
        using class_type = send_email;
        using void_awaitable_type = boost::asio::awaitable<void>;

        explicit send_email(http_handle_parameter handle_parameter);

        [[nodiscard]] void_awaitable_type response();

    private:
        [[nodiscard]] static std::string calculate_hmac_sha256(int64_t app_id, const std::string& email, int64_t timestamp, const std::string& secret_key);

        void send_sdk_email(const email_code& email_code);

        http_handle_parameter handle_parameter_;
    };
}
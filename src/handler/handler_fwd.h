#pragma once

namespace celeritas
{
    class health_check_request_http_message_handler;
    class health_check_response_http_message_handler;
    class discover_response_message_handler;
    class discover_request_message_handler;
    class close_request_message_handler;
    class register_response_message_handler;
    class register_request_message_handler;

    class guest_login_http_message_handler;

    constexpr std::string_view guest_login_path = "/api/v1/login/guest";
    constexpr std::string_view send_sms_path = "/api/v1/login/send_sms";
    constexpr std::string_view send_email_path = "/api/v1/login/send_email";
    constexpr std::string_view phone_login_path = "/api/v1/login/phone";
    constexpr std::string_view email_login_path = "/api/v1/login/email";
    constexpr std::string_view password_login_path = "/api/v1/login/password";
    constexpr std::string_view sdk_login_path = "/api/v1/login/sdk";
    constexpr std::string_view phone_bind_path = "/api/v1/login/bind/phone";
    constexpr std::string_view email_bind_path = "/api/v1/login/bind/email";
    constexpr std::string_view password_bind_path = "/api/v1/login/bind/password";
    constexpr std::string_view sdk_bind_path = "/api/v1/login/bind/sdk";
    constexpr std::string_view login_servers_path = "/api/v1/login/servers";

    constexpr std::string_view order_create_path = "/api/v1/order/create";
    constexpr std::string_view notify_path = "/api/v1/notify/";

    constexpr std::string_view refund_path = "/refund/";
}
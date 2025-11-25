#pragma once

namespace celeritas
{
    class health_check_request_http_message_handler;
    class health_check_response_http_message_handler;
    class health_check;
    class discover_response_message_handler;
    class discover_request_message_handler;
    class close_request_message_handler;
    class register_response_message_handler;
    class register_request_message_handler;

    class guest_login_http_message_handler;

    enum class health_check_level_type;

    constexpr std::string_view guest_login_path = "/login/guest";
    constexpr std::string_view send_sms_path = "/login/send_sms";
    constexpr std::string_view phone_login_path = "/login/phone";
}
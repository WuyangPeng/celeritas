#pragma once

namespace celeritas
{
    class guest_login_response;
    class app_secret;
    class send_sms_response;
    class send_sms;
    class phone_login_response;
    class phone_login;
    class phone_bind_response;
    class phone_bind;
    class send_email_response;
    class send_email;
    class email_login_response;
    class email_login;
    class email_bind_response;
    class email_bind;

    enum class app_status_type;

    constexpr auto sms_code_expiration_time = 300;
    constexpr auto sms_limit_expiration_time = 60;
    constexpr auto email_code_expiration_time = 900;
    constexpr auto email_limit_expiration_time = 60;
    constexpr auto sms_limit_code_begin = 100000;
    constexpr auto sms_limit_code_end = 999999;
    constexpr auto sms_code_retry_count = 5;
    constexpr auto email_code_retry_count = 5;
}
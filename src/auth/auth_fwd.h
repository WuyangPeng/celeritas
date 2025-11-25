#pragma once

namespace celeritas
{
    class guest_login_response;
    class app_secret;
    class send_sms_response;
    class send_sms;

    enum class app_status_type;

    constexpr auto sms_code_expiration_time = 300;
    constexpr auto sms_limit_expiration_time = 60;
    constexpr auto sms_limit_code_begin = 100000;
    constexpr auto sms_limit_code_end = 999999;
}
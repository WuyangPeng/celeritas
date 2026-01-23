#pragma once

namespace celeritas
{
    constexpr auto sms_code_expiration_time = 300;
    constexpr auto sms_limit_expiration_time = 60;
    constexpr auto email_code_expiration_time = 900;
    constexpr auto email_limit_expiration_time = 60;
    constexpr auto sms_limit_code_begin = 100000;
    constexpr auto sms_limit_code_end = 999999;
    constexpr auto email_limit_code_begin = 100000;
    constexpr auto email_limit_code_end = 999999;
    constexpr auto sms_code_retry_count = 5;
    constexpr auto email_code_retry_count = 5;
}
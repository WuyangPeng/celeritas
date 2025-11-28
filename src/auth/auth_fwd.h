#pragma once

#include "common/common_fwd.h"

namespace celeritas
{
    enum class app_status_type;

    class app_secret;
    class app_sms_providers;
    class auth_service_base;
    class token_http_response;
    class auth_login;

    class guest_login;
    class guest_login_response;

    class send_sms;
    class send_sms_response;
    class phone_login;
    class phone_login_response;
    class phone_bind;
    class phone_bind_response;

    class send_email_response;
    class send_email;
    class email_login_response;
    class email_login;
    class email_bind_response;
    class email_bind;
    class password_login_response;
    class password_login;
    class password_bind_response;
    class password_bind;

    constexpr auto sms_code_expiration_time = 300;
    constexpr auto sms_limit_expiration_time = 60;
    constexpr auto email_code_expiration_time = 900;
    constexpr auto email_limit_expiration_time = 60;
    constexpr auto sms_limit_code_begin = 100000;
    constexpr auto sms_limit_code_end = 999999;
    constexpr auto sms_code_retry_count = 5;
    constexpr auto email_code_retry_count = 5;
    constexpr auto http_request_timestamp_expired = minute * 5;
}
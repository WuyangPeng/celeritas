#pragma once

namespace celeritas
{
    enum class app_status_type;
    enum class sdk_process_type;
    enum class server_status_type;

    class app_secret;
    class app_sms_providers;
    class auth_service_base;
    class sdk_providers_key;
    class app_sdk_providers;
    class token_http_response;
    class auth_login;
    class auth_bind;

    class guest_login;
    class guest_login_response;

    class send_sms;
    class send_sms_response;
    class phone_login;
    class phone_login_response;
    class phone_bind;
    class phone_bind_response;

    class send_email;
    class send_email_response;
    class email_login;
    class email_login_response;
    class email_bind;
    class email_bind_response;

    class password_login_response;
    class password_login;
    class password_bind_response;
    class password_bind;

    class sdk_login;
    class sdk_login_response;
    class sdk_bind;
    class sdk_bind_response;

    class connection_info;
    class player_role;
    class login_server_info;
    class login_servers_response;
    class login_servers;

    class server_cell_repository;

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
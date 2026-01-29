#pragma once

namespace celeritas
{
    enum class server_mode_type;

    class app_secret;
    class app_sms_providers;
    class sdk_providers_key;
    class app_sdk_providers;
    class app_email_providers;
    class server_cell_repository;

    enum class app_status_type;
    enum class server_status_type;

    class token_http_response;
    class auth_login;
    class auth_bind;

    enum class sdk_process_type;

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
}
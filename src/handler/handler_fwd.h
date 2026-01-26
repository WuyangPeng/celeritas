#pragma once

namespace celeritas
{
    class close_request_message_handler;
    class discover_request_message_handler;
    class discover_response_message_handler;
    class health_check_request_http_message_handler;
    class health_check_response_http_message_handler;
    class register_request_message_handler;
    class register_response_message_handler;
    class service_heartbeat_request_message_handler;
    class service_heartbeat_response_message_handler;

    class email_bind_http_message_handler;
    class email_login_http_message_handler;
    class guest_login_http_message_handler;
    class login_servers_http_message_handler;
    class password_bind_http_message_handler;
    class password_login_http_message_handler;
    class phone_bind_http_message_handler;
    class phone_login_http_message_handler;
    class reload_app_db_message_handler;
    class reload_email_providers_db_message_handler;
    class reload_sdk_providers_db_message_handler;
    class reload_server_cell_db_message_handler;
    class reload_sms_providers_db_message_handler;
    class sdk_bind_http_message_handler;
    class sdk_login_http_message_handler;
    class send_email_http_message_handler;
    class send_sms_http_message_handler;
    class notify_http_message_handler;
    class order_create_http_message_handler;
    class refund_http_message_handler;
    class reload_sdk_payment_providers_db_message_handler;

    class gateway_client_login_request_message_handler;
    class gateway_client_player_request_message_handler;
    class gateway_client_request_message_handler;
    class gateway_client_response_message_handler;
    class gateway_login_request_message_handler;
    class gateway_request_message_handler;
    class gateway_service_login_response_message_handler;

    class player_concrete_message_handler;
    class change_role_name_message_handler;
    class client_heartbeat_request_message_handler;
    class offline_request_message_handler;
    class reload_config_db_message_handler;
    class reload_game_config_message_handler;
    class service_login_request_message_handler;
}

// 此文件是自动生成，请勿手动修改。

#include "register_handler_helper.h"
#include "handler/generated/handler_generated.h"
#include "message/handler_base/concrete_message_handler.tpp"
#include "message/registry/http_message_registry.h"
#include "message/registry/protobuf_message_registry.h"

celeritas::register_handler_helper::register_handler_helper(message_registry_shared_ptr message_registry, http_message_registry_shared_ptr http_message_registry)
    : message_registry_{ std::move(message_registry) }, http_message_registry_{ std::move(http_message_registry) }
{
}

void celeritas::register_handler_helper::register_handler() const
{
    message_registry_->register_handler(std::make_shared<reload_app_db_message_handler>());
    message_registry_->register_handler(std::make_shared<reload_email_providers_db_message_handler>());
    message_registry_->register_handler(std::make_shared<reload_sdk_providers_db_message_handler>());
    message_registry_->register_handler(std::make_shared<reload_server_cell_db_message_handler>());
    message_registry_->register_handler(std::make_shared<reload_sms_providers_db_message_handler>());
    message_registry_->register_handler(std::make_shared<celeritas_message_handler>());
    message_registry_->register_handler(std::make_shared<client_debug_request_message_handler>());
    message_registry_->register_handler(std::make_shared<client_debug_response_message_handler>());
    message_registry_->register_handler(std::make_shared<client_develop_request_message_handler>());
    message_registry_->register_handler(std::make_shared<client_develop_response_message_handler>());
    message_registry_->register_handler(std::make_shared<client_heartbeat_request_message_handler>());
    message_registry_->register_handler(std::make_shared<client_heartbeat_response_message_handler>());
    message_registry_->register_handler(std::make_shared<client_item_request_message_handler>());
    message_registry_->register_handler(std::make_shared<client_item_response_message_handler>());
    message_registry_->register_handler(std::make_shared<client_login_request_message_handler>());
    message_registry_->register_handler(std::make_shared<client_login_response_message_handler>());
    message_registry_->register_handler(std::make_shared<client_mail_request_message_handler>());
    message_registry_->register_handler(std::make_shared<client_mail_response_message_handler>());
    message_registry_->register_handler(std::make_shared<client_player_request_message_handler>());
    message_registry_->register_handler(std::make_shared<client_player_response_message_handler>());
    message_registry_->register_handler(std::make_shared<client_red_dot_response_message_handler>());
    message_registry_->register_handler(std::make_shared<client_request_message_handler>());
    message_registry_->register_handler(std::make_shared<client_response_message_handler>());
    message_registry_->register_handler(std::make_shared<client_role_request_message_handler>());
    message_registry_->register_handler(std::make_shared<client_role_response_message_handler>());
    message_registry_->register_handler(std::make_shared<request_message_handler>());
    message_registry_->register_handler(std::make_shared<response_message_handler>());
    message_registry_->register_handler(std::make_shared<service_auth_request_message_handler>());
    message_registry_->register_handler(std::make_shared<service_auth_response_message_handler>());
    message_registry_->register_handler(std::make_shared<service_log_message_handler>());
    message_registry_->register_handler(std::make_shared<service_payment_request_message_handler>());
    message_registry_->register_handler(std::make_shared<service_payment_response_message_handler>());
    message_registry_->register_handler(std::make_shared<service_player_request_message_handler>());
    message_registry_->register_handler(std::make_shared<service_player_response_message_handler>());
    message_registry_->register_handler(std::make_shared<service_registry_request_message_handler>());
    message_registry_->register_handler(std::make_shared<service_registry_response_message_handler>());
    message_registry_->register_handler(std::make_shared<service_request_message_handler>());
    message_registry_->register_handler(std::make_shared<service_response_message_handler>());
    message_registry_->register_handler(std::make_shared<service_log_request_message_handler>());
    message_registry_->register_handler(std::make_shared<reload_sdk_payment_providers_db_message_handler>());
    message_registry_->register_handler(std::make_shared<activate_item_message_handler>());
    message_registry_->register_handler(std::make_shared<change_role_name_message_handler>());
    message_registry_->register_handler(std::make_shared<debug_message_handler>());
    message_registry_->register_handler(std::make_shared<develop_claim_reward_message_handler>());
    message_registry_->register_handler(std::make_shared<develop_level_message_handler>());
    message_registry_->register_handler(std::make_shared<develop_reset_message_handler>());
    message_registry_->register_handler(std::make_shared<heartbeat_request_message_handler>());
    message_registry_->register_handler(std::make_shared<item_selected_message_handler>());
    message_registry_->register_handler(std::make_shared<lock_item_message_handler>());
    message_registry_->register_handler(std::make_shared<mail_collect_all_attachments_message_handler>());
    message_registry_->register_handler(std::make_shared<mail_collect_attachment_message_handler>());
    message_registry_->register_handler(std::make_shared<mail_delete_all_read_message_handler>());
    message_registry_->register_handler(std::make_shared<mail_delete_message_handler>());
    message_registry_->register_handler(std::make_shared<mail_read_message_handler>());
    message_registry_->register_handler(std::make_shared<mail_sync_message_handler>());
    message_registry_->register_handler(std::make_shared<offline_request_message_handler>());
    message_registry_->register_handler(std::make_shared<reload_config_db_message_handler>());
    message_registry_->register_handler(std::make_shared<reload_game_config_message_handler>());
    message_registry_->register_handler(std::make_shared<send_server_mail_message_handler>());
    message_registry_->register_handler(std::make_shared<service_login_request_message_handler>());
    message_registry_->register_handler(std::make_shared<close_request_message_handler>());
    message_registry_->register_handler(std::make_shared<discover_request_message_handler>());
    message_registry_->register_handler(std::make_shared<discover_response_message_handler>());
    message_registry_->register_handler(std::make_shared<register_request_message_handler>());
    message_registry_->register_handler(std::make_shared<register_response_message_handler>());
    message_registry_->register_handler(std::make_shared<service_heartbeat_request_message_handler>());
    message_registry_->register_handler(std::make_shared<service_heartbeat_response_message_handler>());


    http_message_registry_->register_handler(std::make_shared<admin_create_http_message_handler>());
    http_message_registry_->register_handler(std::make_shared<admin_delete_http_message_handler>());
    http_message_registry_->register_handler(std::make_shared<admin_list_http_message_handler>());
    http_message_registry_->register_handler(std::make_shared<admin_login_http_message_handler>());
    http_message_registry_->register_handler(std::make_shared<admin_update_password_http_message_handler>());
    http_message_registry_->register_handler(std::make_shared<admin_update_status_http_message_handler>());
    http_message_registry_->register_handler(std::make_shared<announcement_admin_broadcast_http_message_handler>());
    http_message_registry_->register_handler(std::make_shared<announcement_admin_create_http_message_handler>());
    http_message_registry_->register_handler(std::make_shared<announcement_admin_delete_http_message_handler>());
    http_message_registry_->register_handler(std::make_shared<announcement_admin_list_http_message_handler>());
    http_message_registry_->register_handler(std::make_shared<announcement_admin_update_http_message_handler>());
    http_message_registry_->register_handler(std::make_shared<feedback_admin_list_http_message_handler>());
    http_message_registry_->register_handler(std::make_shared<announcement_client_http_message_handler>());
    http_message_registry_->register_handler(std::make_shared<announcement_red_dot_http_message_handler>());
    http_message_registry_->register_handler(std::make_shared<email_bind_http_message_handler>());
    http_message_registry_->register_handler(std::make_shared<email_login_http_message_handler>());
    http_message_registry_->register_handler(std::make_shared<feedback_submit_http_message_handler>());
    http_message_registry_->register_handler(std::make_shared<guest_login_http_message_handler>());
    http_message_registry_->register_handler(std::make_shared<login_servers_http_message_handler>());
    http_message_registry_->register_handler(std::make_shared<password_bind_http_message_handler>());
    http_message_registry_->register_handler(std::make_shared<password_login_http_message_handler>());
    http_message_registry_->register_handler(std::make_shared<phone_bind_http_message_handler>());
    http_message_registry_->register_handler(std::make_shared<phone_login_http_message_handler>());
    http_message_registry_->register_handler(std::make_shared<sdk_bind_http_message_handler>());
    http_message_registry_->register_handler(std::make_shared<sdk_login_http_message_handler>());
    http_message_registry_->register_handler(std::make_shared<send_email_http_message_handler>());
    http_message_registry_->register_handler(std::make_shared<send_sms_http_message_handler>());
    http_message_registry_->register_handler(std::make_shared<order_create_http_message_handler>());

}


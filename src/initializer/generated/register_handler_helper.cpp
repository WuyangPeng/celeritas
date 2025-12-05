// 此文件是自动生成，请勿手动修改。

#include "register_handler_helper.h"
#include "handler/generated/handler_generated.h"
#include "message/concrete_message_handler.tpp"
#include "message/http_message_registry.h"
#include "message/protobuf_message_registry.h"

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
    message_registry_->register_handler(std::make_shared<client_auth_request_message_handler>());
    message_registry_->register_handler(std::make_shared<client_auth_response_message_handler>());
    message_registry_->register_handler(std::make_shared<client_request_message_handler>());
    message_registry_->register_handler(std::make_shared<client_response_message_handler>());
    message_registry_->register_handler(std::make_shared<request_message_handler>());
    message_registry_->register_handler(std::make_shared<response_message_handler>());
    message_registry_->register_handler(std::make_shared<service_auth_request_message_handler>());
    message_registry_->register_handler(std::make_shared<service_auth_response_message_handler>());
    message_registry_->register_handler(std::make_shared<service_payment_request_message_handler>());
    message_registry_->register_handler(std::make_shared<service_payment_response_message_handler>());
    message_registry_->register_handler(std::make_shared<service_registry_request_message_handler>());
    message_registry_->register_handler(std::make_shared<service_registry_response_message_handler>());
    message_registry_->register_handler(std::make_shared<service_request_message_handler>());
    message_registry_->register_handler(std::make_shared<service_response_message_handler>());
    message_registry_->register_handler(std::make_shared<reload_sdk_payment_providers_db_message_handler>());
    message_registry_->register_handler(std::make_shared<close_request_message_handler>());
    message_registry_->register_handler(std::make_shared<discover_request_message_handler>());
    message_registry_->register_handler(std::make_shared<discover_response_message_handler>());
    message_registry_->register_handler(std::make_shared<register_request_message_handler>());
    message_registry_->register_handler(std::make_shared<register_response_message_handler>());


    http_message_registry_->register_handler(std::make_shared<email_bind_http_message_handler>());
    http_message_registry_->register_handler(std::make_shared<email_login_http_message_handler>());
    http_message_registry_->register_handler(std::make_shared<guest_login_http_message_handler>());
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


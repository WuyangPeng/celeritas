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
    message_registry_->registerHandler(std::make_shared<reload_app_db_message_handler>());
    message_registry_->registerHandler(std::make_shared<auth_request_message_handler>());
    message_registry_->registerHandler(std::make_shared<auth_response_message_handler>());
    message_registry_->registerHandler(std::make_shared<celeritas_message_handler>());
    message_registry_->registerHandler(std::make_shared<request_message_handler>());
    message_registry_->registerHandler(std::make_shared<response_message_handler>());
    message_registry_->registerHandler(std::make_shared<service_registry_request_message_handler>());
    message_registry_->registerHandler(std::make_shared<service_registry_response_message_handler>());
    message_registry_->registerHandler(std::make_shared<service_request_message_handler>());
    message_registry_->registerHandler(std::make_shared<service_response_message_handler>());
    message_registry_->registerHandler(std::make_shared<close_request_message_handler>());
    message_registry_->registerHandler(std::make_shared<discover_request_message_handler>());
    message_registry_->registerHandler(std::make_shared<discover_response_message_handler>());
    message_registry_->registerHandler(std::make_shared<register_request_message_handler>());
    message_registry_->registerHandler(std::make_shared<register_response_message_handler>());


    http_message_registry_->registerHandler(std::make_shared<guest_login_http_message_handler>());
    http_message_registry_->registerHandler(std::make_shared<send_sms_http_message_handler>());

}


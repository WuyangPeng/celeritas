#include "register_handler_helper.h"
#include "handler/generated/handler_generated.h"
#include "message/message_registry.h"
#include "message/http_message_registry.h"

celeritas::register_handler_helper::register_handler_helper(const message_registry_shared_ptr& message_registry, const http_message_registry_shared_ptr& http_message_registry)
    : message_registry_{ message_registry }, http_message_registry_{ http_message_registry }
{
}

void celeritas::register_handler_helper::register_handler() const
{
    message_registry_->registerHandler(std::make_shared<request_message_handler>());
    message_registry_->registerHandler(std::make_shared<response_message_handler>());
    message_registry_->registerHandler(std::make_shared<service_request_message_handler>());
    message_registry_->registerHandler(std::make_shared<service_response_message_handler>());
}


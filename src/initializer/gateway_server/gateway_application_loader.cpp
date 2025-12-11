#include "gateway_application_loader.h"
#include "handler/gateway/manual/gateway_client_login_request_message_handler.h"
#include "handler/gateway/manual/gateway_login_request_message_handler.h"
#include "handler/gateway/manual/gateway_client_player_request_message_handler.h"
#include "handler/gateway/manual/gateway_request_message_handler.h"
#include "handler/gateway/manual/gateway_client_request_message_handler.h"
#include "handler/gateway/manual/gateway_client_response_message_handler.h"
#include "handler/gateway/manual/gateway_service_login_response_message_handler.h"
#include "message/concrete_message_handler.tpp"

celeritas::gateway_application_loader::gateway_application_loader(app_config_shared_ptr app_config)
    : base_type{ std::move(app_config) }
{
}

void celeritas::gateway_application_loader::service_initialize_application()
{
    register_message_handler();
}

void celeritas::gateway_application_loader::register_message_handler()
{
    register_handler(std::make_shared<gateway_request_message_handler>());
    register_handler(std::make_shared<gateway_client_request_message_handler>());
    register_handler(std::make_shared<gateway_client_response_message_handler>());
    register_handler(std::make_shared<gateway_client_player_request_message_handler>());
    register_handler(std::make_shared<gateway_client_login_request_message_handler>());
    register_handler(std::make_shared<gateway_login_request_message_handler>());
    register_handler(std::make_shared<gateway_service_login_response_message_handler>());
}
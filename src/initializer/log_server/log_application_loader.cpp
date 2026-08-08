#include "log_application_loader.h"
#include "message/handler_base/concrete_message_handler.tpp"

celeritas::log_application_loader::log_application_loader(const std::string_view server_type, app_config_shared_ptr app_config)
    : base_type{ server_type, std::move(app_config) }
{
}

void celeritas::log_application_loader::service_initialize_application()
{
    register_message_handler();
}

void celeritas::log_application_loader::register_message_handler()
{
}
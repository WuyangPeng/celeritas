#include "game_application_loader.h"

celeritas::game_application_loader::game_application_loader(app_config_shared_ptr app_config)
    : base_type{ std::move(app_config) }
{
}

void celeritas::game_application_loader::service_initialize_application()
{
    register_message_handler();
}

void celeritas::game_application_loader::register_message_handler()
{
}
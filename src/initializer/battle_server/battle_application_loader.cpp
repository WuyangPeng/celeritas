#include "battle_application_loader.h"

celeritas::battle_application_loader::battle_application_loader(app_config_shared_ptr app_config)
    : base_type{ std::move(app_config) }
{
}

void celeritas::battle_application_loader::service_initialize_application()
{
}

void celeritas::battle_application_loader::register_message_handler()
{
}
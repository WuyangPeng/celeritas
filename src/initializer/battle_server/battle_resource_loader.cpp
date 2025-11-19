#include "battle_resource_loader.h"

celeritas::battle_resource_loader::battle_resource_loader(app_config_shared_ptr app_config)
    : base_type{ std::move(app_config) }
{
}

void celeritas::battle_resource_loader::service_initialize_resource(io_context_type& io_context, const network_message_callback_weak_ptr& network_message_callback)
{
}
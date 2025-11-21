#include "chat_resource_loader.h"

celeritas::chat_resource_loader::chat_resource_loader(const std::string_view server_type, app_config_shared_ptr app_config)
    : base_type{ server_type, std::move(app_config) }
{
}

void celeritas::chat_resource_loader::service_initialize_resource(io_context_type& io_context, const network_message_callback_weak_ptr& network_message_callback)
{
}
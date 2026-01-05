#include "cross_resource_loader.h"

celeritas::cross_resource_loader::cross_resource_loader(const std::string_view server_type, const_app_config_shared_ptr app_config)
    : base_type{ server_type, std::move(app_config) }
{
}

void celeritas::cross_resource_loader::service_initialize_resource(const any_io_executor& any_io_executor, const network_message_callback_weak_ptr& network_message_callback)
{
}
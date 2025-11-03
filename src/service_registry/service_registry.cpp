#include "service_registry.h"
#include "detail/service_registry_impl.h"

void celeritas::service_registry::register_service(const service_info& info)
{
    get_service_registry_impl().register_service(info);
}

celeritas::service_registry::service_info_container_type celeritas::service_registry::get_services(const std::string& service_name)
{
    return get_service_registry_impl().get_services(service_name);
}

void celeritas::service_registry::start_cleanup_timer(io_context_type& io_context)
{
    return get_service_registry_impl().start_cleanup_timer(io_context);
}

void celeritas::service_registry::remove_instance(const std::string& instance_id)
{
    return get_service_registry_impl().remove_instance(instance_id);
}

celeritas::service_registry::registry_type celeritas::service_registry::get_services()
{
    return get_service_registry_impl().get_services();
}

celeritas::service_registry_impl& celeritas::service_registry::get_service_registry_impl()
{
    static service_registry_impl service_registry_impl{};

    return service_registry_impl;
}
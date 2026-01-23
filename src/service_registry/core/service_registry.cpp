#include "service_registry.h"
#include "detail/service_registry_impl.h"

void celeritas::service_registry::register_service(const service_info& info)
{
    get_service_registry_impl().register_service(info);
}

void celeritas::service_registry::clear_services(const std::string& service_name)
{
    return get_service_registry_impl().clear_services(service_name);
}

celeritas::service_registry::service_info_container_type celeritas::service_registry::get_services(const std::string& service_name)
{
    return get_service_registry_impl().get_services(service_name);
}

celeritas::service_registry::service_info_container_type celeritas::service_registry::get_idle_services(const std::string& service_name)
{
    return get_service_registry_impl().get_idle_services(service_name);
}

celeritas::service_registry::optional_service_info celeritas::service_registry::get_idle_services(const std::string& service_name, const std::string& game_server_id)
{
    return get_service_registry_impl().get_idle_services(service_name, game_server_id);
}

celeritas::service_registry::optional_service_info celeritas::service_registry::get_services_by_instance_id(const std::string& instance_id)
{
    return get_service_registry_impl().get_services_by_instance_id(instance_id);
}

void celeritas::service_registry::start_cleanup_timer(const any_io_executor& any_io_executor)
{
    return get_service_registry_impl().start_cleanup_timer(any_io_executor);
}

void celeritas::service_registry::remove_instance(const std::string& instance_id)
{
    return get_service_registry_impl().remove_instance(instance_id);
}

void celeritas::service_registry::set_service_health(const std::string& instance_id, const health_check_level_type health_check_level)
{
    return get_service_registry_impl().set_service_health(instance_id, health_check_level);
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
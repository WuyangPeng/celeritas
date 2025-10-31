#include "service_registry_resource_loader.h"

#include <boost/polymorphic_pointer_cast.hpp>

celeritas::service_registry_resource_loader::service_registry_resource_loader(app_config_shared_ptr app_config)
    : base_type{ std::move(app_config) }, health_check_timer_{}
{
}

void celeritas::service_registry_resource_loader::service_initialize_resource(io_context_type& io_context)
{
    start_health_check_timer(io_context);
}

void celeritas::service_registry_resource_loader::start_health_check_timer(io_context_type& io_context)
{
    const auto app_config = get_app_config();
    const auto health_check_url_config = app_config->get_health_check_url_config();

    health_check_timer_ = std::make_unique<health_check_timer>(io_context, std::chrono::seconds(health_check_url_config.get_interval()), boost::polymorphic_pointer_downcast<class_type>(shared_from_this()));

    health_check_timer_->start(true);
}
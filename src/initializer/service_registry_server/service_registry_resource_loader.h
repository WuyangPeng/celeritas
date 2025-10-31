#pragma once

#include "detail/health_check_timer.h"
#include "initializer/resource_loader.h"

namespace celeritas
{
    class service_registry_resource_loader final : public resource_loader
    {
    public:
        using class_type = service_registry_resource_loader;
        using base_type = resource_loader;

        explicit service_registry_resource_loader(app_config_shared_ptr app_config);

    private:
        using health_check_timer_shared_ptr = std::shared_ptr<health_check_timer>;

        void service_initialize_resource(io_context_type& io_context) override;

        void start_health_check_timer(io_context_type& io_context);

        health_check_timer_shared_ptr health_check_timer_;
    };
}

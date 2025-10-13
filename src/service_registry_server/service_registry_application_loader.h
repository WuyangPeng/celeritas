#pragma once

#include "initializer/application_loader.h"

namespace celeritas
{
    class service_registry_application_loader : public application_loader
    {
    public:
        using class_type = service_registry_application_loader;
        using base_type = application_loader;

        explicit service_registry_application_loader(const app_config_shared_ptr& app_config);

    private:
        void service_initialize_application() override;
    };
}



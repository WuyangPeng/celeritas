#pragma once

#include "initializer/resource_loader.h"

namespace celeritas
{
    class service_registry_resource_loader final : public resource_loader
    {
    public:
        using class_type = service_registry_resource_loader;
        using base_type = resource_loader;

        explicit service_registry_resource_loader(app_config app_config);

    private:
        void service_initialize_resource() override;
    };
}

#pragma once

#include "initializer/resource_loader.h"

namespace celeritas
{
    class chat_resource_loader final : public resource_loader
    {
    public:
        using class_type = chat_resource_loader;
        using base_type = resource_loader;

        explicit chat_resource_loader(app_config_shared_ptr app_config);

    private:
        void service_initialize_resource() override;
    };
}

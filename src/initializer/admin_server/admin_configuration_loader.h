#pragma once

#include "initializer/configuration_loader.h"

namespace celeritas
{
    class admin_configuration_loader final : public configuration_loader
    {
    public:
        using class_type = admin_configuration_loader;
        using base_type = configuration_loader;

        explicit admin_configuration_loader(std::string config_file_path);

    private:
        void service_initialize_config() override;
    };
}

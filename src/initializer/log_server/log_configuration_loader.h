#pragma once

#include "initializer/configuration_loader.h"

namespace celeritas
{
    class log_configuration_loader final : public configuration_loader
    {
    public:
        using class_type = log_configuration_loader;
        using base_type = configuration_loader;

        explicit log_configuration_loader(std::string config_file_path);

    private:
        void service_initialize_config() override;
    };
}

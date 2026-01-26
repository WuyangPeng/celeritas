#pragma once

#include "initializer/loader_base/configuration_loader.h"

namespace celeritas
{
    class chat_configuration_loader final : public configuration_loader
    {
    public:
        using class_type = chat_configuration_loader;
        using base_type = configuration_loader;

        explicit chat_configuration_loader(std::string config_file_path);

    private:
        void service_initialize_config() override;
    };
}

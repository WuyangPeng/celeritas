#pragma once

#include "initializer/initializer.h"

namespace celeritas
{
    class service_registry_initializer final : public initializer
    {
    public:
        using class_type = service_registry_initializer;
        using base_type = initializer;

        explicit service_registry_initializer(std::string config_file_path) noexcept;

        void initialize_config() override;

        void initialize_resource() override;

        void initialize_application() override;
    };
}

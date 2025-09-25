#pragma once

#include "initializer/initializer.h"

namespace celeritas
{
    class service_registry_initializer final : public initializer
    {
    public:
        using class_type = service_registry_initializer;
        using base_type = initializer;

        explicit service_registry_initializer(std::string_view config_file_path, boost::asio::io_context& io_context) noexcept;

        void service_initialize_config() override;

        void service_initialize_resource() override;

        void service_initialize_application() override;
    };
}

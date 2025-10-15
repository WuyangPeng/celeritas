#pragma once

#include "application_loader.h"
#include "configuration_loader.h"
#include "resource_loader.h"

#include <memory>

namespace celeritas
{
    class initializer_factory
    {
    public:
        using class_type = initializer_factory;
        using configuration_loader_unique_ptr = std::unique_ptr<configuration_loader>;
        using resource_loader_unique_ptr = std::unique_ptr<resource_loader>;
        using application_loader_unique_ptr = std::unique_ptr<application_loader>;
        using app_config_shared_ptr = std::shared_ptr<app_config>;

        [[nodiscard]] static configuration_loader_unique_ptr create_configuration_loader(const std::string_view& server_type, std::string config_file_path);

        [[nodiscard]] static resource_loader_unique_ptr create_resource_loader(const std::string_view& server_type, const app_config_shared_ptr& app_config);

        [[nodiscard]] static application_loader_unique_ptr create_application_loader(const std::string_view& server_type, const app_config_shared_ptr& app_config);
    };
}

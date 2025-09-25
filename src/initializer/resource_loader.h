#pragma once
#include "config/app_config.h"

namespace celeritas
{
    class resource_loader
    {
    public:
        using class_type = resource_loader;

        explicit resource_loader(const app_config& app_config);

        virtual ~resource_loader() noexcept = default;

        resource_loader(const resource_loader& rhs) noexcept = delete;

        resource_loader& operator=(const resource_loader& rhs) noexcept = delete;

        resource_loader(resource_loader&& rhs) noexcept = delete;

        resource_loader& operator=(resource_loader&& rhs) noexcept = delete;

        void initialize();

    private:
        void initialize_logger_resource();

        void initialize_database_resource();

        void initialize_server_resource();

        void initialize_health_check_url_resource();

        void initialize_service_registry_resource();

        virtual void service_initialize_resource() = 0;

        app_config app_config_;
    };
}
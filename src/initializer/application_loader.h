#pragma once

#include "config/app_config.h"
#include "worker_pool/worker_pool.h"

namespace celeritas
{
    class application_loader
    {
    public:
        using class_type = application_loader;
        using app_config_shared_ptr = std::shared_ptr<app_config>;

        explicit application_loader(app_config_shared_ptr app_config);

        virtual ~application_loader() noexcept = default;

        application_loader(const application_loader& rhs) noexcept = delete;

        application_loader& operator=(const application_loader& rhs) noexcept = delete;

        application_loader(application_loader&& rhs) noexcept = delete;

        application_loader& operator=(application_loader&& rhs) noexcept = delete;

        void initialize();

        void stop();

    private:
        using worker_pool_unique_ptr = std::unique_ptr<worker_pool>;

        void initialize_worker_pool();

        virtual void service_initialize_application() = 0;

        app_config_shared_ptr app_config_;
        worker_pool_unique_ptr worker_pool_;
    };
}

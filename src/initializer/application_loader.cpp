#include "application_loader.h"
#include "service_registry/service_registry.h"

celeritas::application_loader::application_loader(app_config_shared_ptr app_config)
    : app_config_{ std::move(app_config) }, worker_pool_{}
{
}

void celeritas::application_loader::initialize()
{
    initialize_worker_pool();

    service_initialize_application();
}

void celeritas::application_loader::stop()
{
    worker_pool_.reset();
}

void celeritas::application_loader::initialize_worker_pool()
{
    auto work_pool_size = app_config_->get_server_config().get_worker_pool_size();

    work_pool_size = std::max(min_worker_pool_size, work_pool_size);

    worker_pool_ = std::make_unique<worker_pool>(work_pool_size);
}
#include "application_loader.h"
#include "message/service_request_message_handler.h"
#include "service_registry/service_registry.h"

celeritas::application_loader::application_loader(app_config_shared_ptr app_config)
    : app_config_{ std::move(app_config) }, worker_pool_{}, message_registry_{ std::make_shared<message_registry>() }
{
}

void celeritas::application_loader::initialize()
{
    initialize_worker_pool();
    initialize_message_registry();

    service_initialize_application();
}

void celeritas::application_loader::stop()
{
    worker_pool_.reset();
}

void celeritas::application_loader::register_handler(const base_message_handler_shared_ptr& handler)
{
    message_registry_->registerHandler(handler);
}

bool celeritas::application_loader::dispatch(const header& header, const google::protobuf::Message& current_message, const protobuf_message_shared_ptr& request_message)
{
    return message_registry_->dispatch(header, current_message, request_message);
}

celeritas::application_loader::message_registry_weak_ptr celeritas::application_loader::get_message_registry()
{
    return message_registry_;
}

void celeritas::application_loader::initialize_worker_pool()
{
    auto work_pool_size = app_config_->get_server_config().get_worker_pool_size();

    work_pool_size = std::max(min_worker_pool_size, work_pool_size);

    worker_pool_ = std::make_unique<worker_pool>(work_pool_size);
}

void celeritas::application_loader::initialize_message_registry()
{
    message_registry_->registerHandler(std::make_shared<service_request_message_handler>());
}
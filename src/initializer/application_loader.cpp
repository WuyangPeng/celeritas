#include "application_loader.h"
#include "message/message_handler/health_check_http_message_handler.h"
#include "message/http_handle_parameter.h"
#include "message/http_message_registry.h"
#include "message/message_handler/request_message_handler.h"
#include "message/message_handler/response_message_handler.h"
#include "message/message_handler/service_request_message_handler.h"
#include "message/message_handler/service_response_message_handler.h"

celeritas::application_loader::application_loader(app_config_shared_ptr app_config)
    : app_config_{ std::move(app_config) }, worker_pool_{}, message_registry_{ std::make_shared<message_registry>() }, http_message_registry_{ std::make_shared<http_message_registry>() }
{
}

void celeritas::application_loader::initialize()
{
    initialize_worker_pool();
    initialize_message_registry();
    initialize_health_check();

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

bool celeritas::application_loader::dispatch(const header& header, const protobuf_message& current_message, const protobuf_message_shared_ptr& request_message, const session_shared_ptr& session, const resource_loader_shared_ptr& resource_loader)
{
    return message_registry_->dispatch(handle_parameter{ header, request_message, session, resource_loader }, current_message);
}

bool celeritas::application_loader::dispatch(const std::string& path, const urls_params_view_type& params, const session_shared_ptr& session, const resource_loader_shared_ptr& resource_loader)
{
    return http_message_registry_->dispatch(http_handle_parameter{ path, params, session, resource_loader });
}

celeritas::application_loader::message_registry_weak_ptr celeritas::application_loader::get_message_registry()
{
    return message_registry_;
}

void celeritas::application_loader::initialize_worker_pool()
{
    const auto work_pool_size = std::max(min_worker_pool_size, app_config_->get_server_config().get_worker_pool_size());

    worker_pool_ = std::make_unique<worker_pool>(work_pool_size);
}

void celeritas::application_loader::initialize_message_registry()
{
    message_registry_->registerHandler(std::make_shared<request_message_handler>());
    message_registry_->registerHandler(std::make_shared<response_message_handler>());
    message_registry_->registerHandler(std::make_shared<service_request_message_handler>());
    message_registry_->registerHandler(std::make_shared<service_response_message_handler>());
}

void celeritas::application_loader::initialize_health_check()
{
    const auto health_check_url_config = app_config_->get_health_check_url_config();

    const auto url = health_check_url_config.get_url();

    http_message_registry_->registerHandler(std::make_shared<health_check_http_message_handler>(url));
}
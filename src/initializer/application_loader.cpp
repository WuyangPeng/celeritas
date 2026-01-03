#include "application_loader.h"
#include "generated/register_handler_helper.h"
#include "handler/generated/request_message_handler.h"
#include "handler/generated/response_message_handler.h"
#include "handler/service_registry/manual/health_check_request_http_message_handler.h"
#include "handler/service_registry/manual/health_check_response_http_message_handler.h"
#include "message/http_handle_parameter.h"
#include "message/http_message_registry.h"
#include "message/protobuf_handle_parameter.h"

celeritas::application_loader::application_loader(app_config_shared_ptr app_config)
    : app_config_{ std::move(app_config) }, worker_pool_{}, message_registry_{ std::make_shared<protobuf_message_registry>() }, http_message_registry_{ std::make_shared<http_message_registry>() }
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
    message_registry_->register_handler(handler);
}

void celeritas::application_loader::register_handler(const http_base_message_handler_shared_ptr& handler)
{
    http_message_registry_->register_handler(handler);
}

bool celeritas::application_loader::dispatch(io_context_type& io_context, const header& header, const protobuf_message& current_message, const protobuf_message_shared_ptr& request_message, const session_shared_ptr& session, const resource_loader_shared_ptr& resource_loader)
{
    return message_registry_->dispatch(std::make_shared<protobuf_handle_parameter>(io_context, header, request_message, session, resource_loader, shared_from_this()), current_message);
}

bool celeritas::application_loader::dispatch(io_context_type& io_context, const std::string& path, const urls_params_view_type& params, const session_shared_ptr& session, const resource_loader_shared_ptr& resource_loader)
{
    return http_message_registry_->dispatch(std::make_shared<http_handle_parameter>(io_context, path, params, session, resource_loader, shared_from_this()));
}

bool celeritas::application_loader::dispatch(io_context_type& io_context, const std::string& path, const std::string& params, const session_shared_ptr& session, const resource_loader_shared_ptr& resource_loader)
{
    return http_message_registry_->dispatch(std::make_shared<http_handle_parameter>(io_context, path, params, session, resource_loader, shared_from_this()));
}

celeritas::application_loader::message_registry_weak_ptr celeritas::application_loader::get_message_registry()
{
    return message_registry_;
}

void celeritas::application_loader::submit_task(task_type task)
{
    worker_pool_->submit(std::move(task));
}

void celeritas::application_loader::initialize_worker_pool()
{
    const auto work_pool_size = std::max(min_worker_pool_size, app_config_->get_server_config()->get_worker_pool_size());

    worker_pool_ = std::make_unique<worker_pool>(work_pool_size);
}

void celeritas::application_loader::initialize_message_registry()
{
    const register_handler_helper handler{ message_registry_, http_message_registry_ };

    handler.register_handler();
}

void celeritas::application_loader::initialize_health_check()
{
    const auto health_check_url_config = app_config_->get_health_check_url_config();

    const auto url = health_check_url_config.get_url();

    http_message_registry_->register_handler(std::make_shared<health_check_request_http_message_handler>(url));
    http_message_registry_->register_handler(std::make_shared<health_check_response_http_message_handler>(url));
}
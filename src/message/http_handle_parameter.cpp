#include "http_handle_parameter.h"
#include "common/celeritas_error.h"
#include "initializer/resource_loader.h"

#include <utility>
#include "network/session.h"

celeritas::http_handle_parameter::http_handle_parameter(io_context_type& io_context, std::string path, const urls_params_view_type& params, const session_shared_ptr& session, const resource_loader_shared_ptr& resource_loader)
    : io_context_{ io_context }, path_{ std::move(path) }, params_{ params }, response_{}, session_{ session }, resource_loader_{ resource_loader }
{
}

celeritas::http_handle_parameter::http_handle_parameter(io_context_type& io_context, std::string path, const std::string& params, const session_shared_ptr& session, const resource_loader_shared_ptr& resource_loader)
    : io_context_{ io_context }, path_{ std::move(path) }, params_{}, response_{ params }, session_{ session }, resource_loader_{ resource_loader }
{
}

celeritas::http_handle_parameter::http_handle_parameter(const http_handle_parameter& rhs)
    : io_context_{ rhs.io_context_ }, path_{ rhs.path_ }, params_{ rhs.params_ }, response_{ rhs.response_ }, session_{ rhs.session_ }, resource_loader_{ rhs.resource_loader_ }
{
}

celeritas::http_handle_parameter::http_handle_parameter(http_handle_parameter&& rhs) noexcept
    : io_context_{ rhs.io_context_ }, path_{ std::move(rhs.path_) }, params_{ std::move(rhs.params_) }, response_{ std::move(rhs.response_) }, session_{ std::move(rhs.session_) }, resource_loader_{ std::move(rhs.resource_loader_) }
{
}

std::string celeritas::http_handle_parameter::get_path() const
{
    return path_;
}

std::string celeritas::http_handle_parameter::get_response() const
{
    return response_;
}

void celeritas::http_handle_parameter::write(const std::string& response) const
{
    if (const auto session_shared_ptr = session_.lock();
        session_shared_ptr != nullptr)
    {
        session_shared_ptr->write(response);
    }
}

celeritas::http_handle_parameter::app_config_shared_ptr celeritas::http_handle_parameter::get_app_config() const
{
    if (const auto resource_loader_shared_ptr = resource_loader_.lock();
        resource_loader_shared_ptr != nullptr)
    {
        return resource_loader_shared_ptr->get_app_config();
    }

    throw celeritas_error("resource_loader is null.");
}

celeritas::http_handle_parameter::health_check_level_awaitable_type celeritas::http_handle_parameter::get_health_check_level() const
{
    if (const auto resource_loader_shared_ptr = resource_loader_.lock();
        resource_loader_shared_ptr != nullptr)
    {
        co_return co_await resource_loader_shared_ptr->get_health_check_level();
    }

    throw celeritas_error("resource_loader is null.");
}

celeritas::http_handle_parameter::io_context_type& celeritas::http_handle_parameter::get_io_context() const
{
    return io_context_;
}
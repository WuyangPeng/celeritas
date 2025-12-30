#include "http_handle_parameter.h"
#include "common/application_loader_base.h"
#include "common/core_utilities/celeritas_error.h"
#include "common/logger/logger.h"
#include "common/resource_loader_base.h"
#include "common/session.h"
#include "config/app_config.h"

celeritas::http_handle_parameter::http_handle_parameter(io_context_type& io_context,
                                                        std::string path,
                                                        const urls_params_view_type& params,
                                                        const session_shared_ptr& session,
                                                        const resource_loader_shared_ptr& resource_loader,
                                                        const application_loader_shared_ptr& application_loader)
    : io_context_{ io_context },
      path_{ std::move(path) },
      params_{ params },
      response_{},
      session_{ session },
      resource_loader_{ resource_loader },
      application_loader_{ application_loader }
{
}

celeritas::http_handle_parameter::http_handle_parameter(io_context_type& io_context,
                                                        std::string path,
                                                        std::string params,
                                                        const session_shared_ptr& session,
                                                        const resource_loader_shared_ptr& resource_loader,
                                                        const application_loader_shared_ptr& application_loader)
    : io_context_{ io_context },
      path_{ std::move(path) },
      params_{},
      response_{ std::move(params) },
      session_{ session },
      resource_loader_{ resource_loader },
      application_loader_{ application_loader }
{
}

celeritas::http_handle_parameter::http_handle_parameter(const http_handle_parameter& rhs)
    : io_context_{ rhs.io_context_ },
      path_{ rhs.path_ },
      params_{ rhs.params_ },
      response_{ rhs.response_ },
      session_{ rhs.session_ },
      resource_loader_{ rhs.resource_loader_ },
      application_loader_{ rhs.application_loader_ }
{
}

celeritas::http_handle_parameter::http_handle_parameter(http_handle_parameter&& rhs) noexcept
    : io_context_{ rhs.io_context_ },
      path_{ std::move(rhs.path_) },
      params_{ rhs.params_ },
      response_{ std::move(rhs.response_) },
      session_{ std::move(rhs.session_) },
      resource_loader_{ std::move(rhs.resource_loader_) },
      application_loader_{ std::move(rhs.application_loader_) }
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

celeritas::http_handle_parameter::optional_string celeritas::http_handle_parameter::get_param(const std::string& key) const
{
    if (const auto iter = params_.find(key);
        iter != params_.end())
    {
        if (const auto& param = *iter;
            param.has_value)
        {
            return param.value;
        }
    }

    return std::nullopt;
}

celeritas::http_handle_parameter::void_waitable_type celeritas::http_handle_parameter::write_immediately(const std::string& response) const
{
    if (const auto session_shared_ptr = session_.lock();
        session_shared_ptr != nullptr)
    {
        LOG_CHANNEL(message_channel, debug) << "http start response " << response;

        co_await session_shared_ptr->write_immediately(response);

        co_return session_shared_ptr->remove_session();
    }

    LOG_CHANNEL(message_channel, debug) << "http session is close.";
}

celeritas::http_handle_parameter::app_config_const_shared_ptr celeritas::http_handle_parameter::get_app_config() const
{
    return get_resource_loader()->get_app_config();
}

celeritas::http_handle_parameter::health_check_level_awaitable_type celeritas::http_handle_parameter::get_health_check_level() const
{
    if (const auto resource_loader_shared_ptr = resource_loader_.lock();
        resource_loader_shared_ptr != nullptr)
    {
        co_return co_await resource_loader_shared_ptr->get_health_check_level();
    }

    throw celeritas_error{ "resource loader is null." };
}

celeritas::http_handle_parameter::io_context_type& celeritas::http_handle_parameter::get_io_context() const
{
    return io_context_;
}

std::string_view celeritas::http_handle_parameter::get_server_type() const
{
    return get_resource_loader()->get_server_type();
}

celeritas::database_config celeritas::http_handle_parameter::get_database_config(const std::string& db_name) const
{
    const auto app_config = get_resource_loader()->get_app_config();

    return app_config->get_database_config(db_name);
}

void celeritas::http_handle_parameter::submit_task(task_type task) const
{
    if (const auto application_loader_shared_ptr = application_loader_.lock();
        application_loader_shared_ptr != nullptr)
    {
        return application_loader_shared_ptr->submit_task(std::move(task));
    }

    throw celeritas_error{ "application loader is null." };
}

celeritas::http_handle_parameter::resource_loader_const_shared_ptr celeritas::http_handle_parameter::get_resource_loader() const
{
    if (const auto resource_loader_shared_ptr = resource_loader_.lock();
        resource_loader_shared_ptr != nullptr)
    {
        return resource_loader_shared_ptr;
    }

    throw celeritas_error{ "resource loader is null." };
}
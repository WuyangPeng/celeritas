#include "http_handle_parameter.h"
#include "common/core/celeritas_error.h"
#include "common/framework/application_loader_base.h"
#include "common/framework/resource_loader_base.h"
#include "common/framework/session.h"
#include "common/logging/logger.h"
#include "config/aggregate/app_config.h"

celeritas::http_handle_parameter::http_handle_parameter(std::string path,
                                                        const urls_params_view_type& params,
                                                        const session_shared_ptr& session,
                                                        const resource_loader_shared_ptr& resource_loader,
                                                        const application_loader_shared_ptr& application_loader)
    : path_{ std::move(path) },
      params_{ params },
      response_{},
      session_{ session },
      resource_loader_{ resource_loader },
      application_loader_{ application_loader }
{
}

celeritas::http_handle_parameter::http_handle_parameter(std::string path,
                                                        std::string params,
                                                        const session_shared_ptr& session,
                                                        const resource_loader_shared_ptr& resource_loader,
                                                        const application_loader_shared_ptr& application_loader)
    : path_{ std::move(path) },
      params_{},
      response_{ std::move(params) },
      session_{ session },
      resource_loader_{ resource_loader },
      application_loader_{ application_loader }
{
}

celeritas::http_handle_parameter::http_handle_parameter(const http_handle_parameter& rhs)
    : path_{ rhs.path_ },
      params_{ rhs.params_ },
      response_{ rhs.response_ },
      session_{ rhs.session_ },
      resource_loader_{ rhs.resource_loader_ },
      application_loader_{ rhs.application_loader_ }
{
}

celeritas::http_handle_parameter::http_handle_parameter(http_handle_parameter&& rhs) noexcept
    : path_{ std::move(rhs.path_) },
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

std::string celeritas::http_handle_parameter::get_body() const
{
    return response_; // response_ 即从客户端/服务端传输过来的 body 描述字符串
}

std::string celeritas::http_handle_parameter::get_remote_ip_address() const
{
    if (const auto session = get_session())
    {
        return session->get_remote_ip_address();
    }
    return "";
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

    if (!response_.empty())
    {
        auto raw_body = response_;
        if (raw_body.find('%') != std::string::npos)
        {
            const auto decoded = boost::urls::decode_view(raw_body);
            raw_body = std::string(decoded.begin(), decoded.end());
        }

        if (const auto body_params = boost::urls::parse_query(raw_body))
        {
            if (const auto iter = body_params->find(key);
                iter != body_params->end())
            {
                if (const auto& param = *iter;
                    param.has_value)
                {
                    const auto val_pct = param.value;
                    const auto decoded = boost::urls::decode_view(val_pct);
                    return std::string(decoded.begin(), decoded.end());
                }
            }
        }
    }

    return std::nullopt;
}

celeritas::http_handle_parameter::void_waitable_type celeritas::http_handle_parameter::write_immediately(const std::string& response) const
{
    const auto session_shared_ptr = get_session();

    LOG_CHANNEL(message_channel, debug) << "http start response " << response;

    co_await session_shared_ptr->write_immediately(response);

    co_return session_shared_ptr->remove_session();
}

celeritas::http_handle_parameter::const_app_config_shared_ptr celeritas::http_handle_parameter::get_app_config() const
{
    return get_resource_loader()->get_app_config();
}

celeritas::http_handle_parameter::health_check_level_awaitable_type celeritas::http_handle_parameter::get_health_check_level() const
{
    co_return co_await get_resource_loader()->get_health_check_level();
}

celeritas::http_handle_parameter::any_io_executor celeritas::http_handle_parameter::get_any_io_executor() const
{
    return get_session()->get_any_io_executor();
}

std::string_view celeritas::http_handle_parameter::get_server_type() const
{
    return get_resource_loader()->get_server_type();
}

celeritas::http_handle_parameter::const_database_config_shared_ptr celeritas::http_handle_parameter::get_database_config(const std::string& db_name) const
{
    return get_resource_loader()->get_app_config()->get_database_config(db_name);
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

celeritas::http_handle_parameter::resource_loader_shared_ptr celeritas::http_handle_parameter::get_resource_loader() const
{
    if (const auto resource_loader_shared_ptr = resource_loader_.lock();
        resource_loader_shared_ptr != nullptr)
    {
        return resource_loader_shared_ptr;
    }

    throw celeritas_error{ "resource loader is null." };
}

celeritas::http_handle_parameter::session_shared_ptr celeritas::http_handle_parameter::get_session() const
{
    if (const auto session_shared_ptr = session_.lock();
        session_shared_ptr != nullptr)
    {
        return session_shared_ptr;
    }

    throw celeritas_error{ "http session is close." };
}
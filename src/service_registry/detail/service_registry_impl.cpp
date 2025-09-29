#include "service_registry_impl.h"
#include "service_registry_internal_fwd.h"
#include "boost/asio/deadline_timer.hpp"
#include "common/logger.h"
#include "common/common_fwd.h"

#include <ranges>

void celeritas::service_registry_impl::register_service(const service_info& info)
{
    std::unique_lock lock{ mutex_ };

    if (const auto iter = registry_.find(info.get_instance_id());
        iter != registry_.end())
    {
        iter->second.set_last_heartbeat();
    }
    else
    {
        registry_[info.get_instance_id()] = info;
    }
}

celeritas::service_registry_impl::service_info_container_type celeritas::service_registry_impl::get_services(const std::string& service_name) const
{
    std::shared_lock lock{ mutex_ };

    service_info_container_type services{};
    for (const auto& element : registry_ | std::views::values)
    {
        if (element.get_service_name() == service_name)
        {
            services.emplace_back(element);
        }
    }
    return services;
}

void celeritas::service_registry_impl::start_cleanup_timer(io_context_type& io_context)
{
    cleanup_timer_interval_ = std::make_unique<steady_timer_type>(io_context);

    start_cleanup_timer(shared_from_this());
}

void celeritas::service_registry_impl::start_cleanup_timer(const self_shared_ptr& self) const
{
    cleanup_timer_interval_->expires_at(std::chrono::steady_clock::now() + cleanup_timer);
    cleanup_timer_interval_->async_wait(
        [self](const error_code_type& ec) {
            if (!ec)
            {
                self->cleanup_expired_services(ec);
            }
        });
}

void celeritas::service_registry_impl::cleanup_expired_services(const error_code_type& error_code)
{
    if (error_code == boost::asio::error::operation_aborted)
    {
        return;
    }

    const auto self{ shared_from_this() };

    process_cleanup_logic();

    start_cleanup_timer(self);
}

void celeritas::service_registry_impl::process_cleanup_logic()
{
    try
    {
        cleanup_services_by_duration();
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(service_registry_channel, error) << "Cleanup error: " << error.what();
    }
    catch (...)
    {
        LOG_CHANNEL(service_registry_channel, fatal) << "Cleanup error: an unknown exception";
    }
}

void celeritas::service_registry_impl::cleanup_services_by_duration()
{
    const auto now = std::chrono::steady_clock::now();

    std::unique_lock lock{ mutex_ };

    for (auto iter = registry_.begin(); iter != registry_.end();)
    {
        if (cleanup_service_entry(iter, now))
        {
            iter = registry_.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}

bool celeritas::service_registry_impl::cleanup_service_entry(const registry_type_iterator& iter, const time_point_type& now)
{
    const auto last_heartbeat = iter->second.get_last_heartbeat();
    if (const auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - last_heartbeat).count();
        duration > services_heartbeat_remove_time)
    {
        LOG_CHANNEL(service_registry_channel, error)
                << "Service removed after "
                << duration
                << "s timeout: "
                << iter->second.get_service_name()
                << " (id: "
                << iter->second.get_instance_id()
                << ")";
        return true;
    }
    else if (duration > services_heartbeat_error_time)
    {
        LOG_CHANNEL(service_registry_channel, error)
                << "Service unresponsive for "
                << duration
                << "s: "
                << iter->second.get_service_name()
                << " (id: "
                << iter->second.get_instance_id()
                << ")";
    }
    else if (duration > services_heartbeat_warning_time)
    {
        LOG_CHANNEL(service_registry_channel, warning)
                << "Service unresponsive for "
                << duration
                << "s: "
                << iter->second.get_service_name()
                << " (id: "
                << iter->second.get_instance_id()
                << ")";
    }
    return false;
}
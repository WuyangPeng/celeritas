#include "cleanup_timer.h"
#include "service_registry_impl.h"
#include "service_registry_internal_fwd.h"
#include "common/logger.h"

#include <ranges>

void celeritas::service_registry_impl::register_service(const service_info& info)
{
    std::lock_guard lock{ mutex_ };

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

celeritas::service_registry_impl::service_info_container_type celeritas::service_registry_impl::get_services(const std::string& service_name)
{
    std::lock_guard lock{ mutex_ };

    service_info_container_type services{};
    for (const auto& element : registry_ | std::views::values)
    {
        if (element.get_service_name() == service_name && element.is_heartbeat())
        {
            services.emplace_back(element);
        }
    }
    return services;
}

void celeritas::service_registry_impl::start_cleanup_timer(io_context_type& io_context)
{
    std::lock_guard lock{ cleanup_timer_mutex_ };

    cleanup_timer_ = std::make_shared<cleanup_timer>(io_context, cleanup_seconds, shared_from_this());

    cleanup_timer_->start();
}

void celeritas::service_registry_impl::cleanup_services_by_duration()
{
    const auto now = std::chrono::steady_clock::now();

    std::lock_guard lock{ mutex_ };

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
    if (const auto duration = std::chrono::duration_cast<seconds_type>(now - last_heartbeat).count();
        duration > services_heartbeat_remove_time)
    {
        log_server_unresponsive(iter, duration, boost::log::trivial::error, "removed after ");
        return true;
    }
    else if (duration > services_heartbeat_error_time)
    {
        log_server_unresponsive(iter, duration, boost::log::trivial::error, "unresponsive for ");
    }
    else if (duration > services_heartbeat_warning_time)
    {
        log_server_unresponsive(iter, duration, boost::log::trivial::warning, "unresponsive for ");
    }

    return false;
}

void celeritas::service_registry_impl::log_server_unresponsive(const registry_type_iterator& iter,
                                                               const int64_t duration,
                                                               const severity_level_type level,
                                                               const std::string& description)
{
    if (level == severity_level_type::warning)
    {
        LOG_CHANNEL(service_registry_channel, warning)
               << "Service "
               << description
               << duration
               << "s: timeout: "
               << iter->second.get_service_name()
               << " (id: "
               << iter->second.get_instance_id()
               << ")";
    }
    else
    {
        LOG_CHANNEL(service_registry_channel, error)
               << "Service "
               << description
               << duration
               << "s: timeout: "
               << iter->second.get_service_name()
               << " (id: "
               << iter->second.get_instance_id()
               << ")";
    }
}
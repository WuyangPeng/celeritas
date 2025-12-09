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
        iter->second.set_last_heartbeat(info.get_start_server_time());
    }
    else
    {
        registry_[info.get_instance_id()] = info;
    }

    server_[info.get_service_name()][info.get_game_server_id()].emplace_back(info);
}

void celeritas::service_registry_impl::clear_services(const std::string& service_name)
{
    std::lock_guard lock{ mutex_ };

    erase_if(registry_, [service_name](const auto& element) {
        return element.second.get_service_name() == service_name;
    });

    server_.erase(service_name);
}

celeritas::service_registry_impl::service_info_container_type celeritas::service_registry_impl::get_services(const std::string& service_name)
{
    std::lock_guard lock{ mutex_ };

    service_info_container_type services{};
    if (const auto iter = server_.find(service_name);
        iter != server_.end())
    {
        for (const auto& container : iter->second | std::views::values)
        {
            for (const auto& element : container)
            {
                if (element.get_health_check_level_type() != health_check_level_type::crash)
                {
                    services.emplace_back(element);
                }
            }
        }
    }

    return services;
}

celeritas::service_registry_impl::service_info_container_type celeritas::service_registry_impl::get_idle_services(const std::string& service_name)
{
    std::lock_guard lock{ mutex_ };

    ++next_index_;

    service_info_container_type services{};
    if (const auto iter = server_.find(service_name);
        iter != server_.end())
    {
        for (const auto& container : iter->second | std::views::values)
        {
            if (const auto service_info = get_idle_services(container))
            {
                services.emplace_back(*service_info);
            }
        }
    }

    return services;
}

celeritas::service_registry_impl::optional_service_info celeritas::service_registry_impl::get_idle_services(const std::string& service_name, const std::string& game_server_id)
{
    std::lock_guard lock{ mutex_ };

    ++next_index_;

    if (const auto iter = server_.find(service_name);
        iter != server_.cend())
    {
        if (const auto game_server = iter->second.find(game_server_id);
            game_server != iter->second.cend())
        {
            return get_idle_services(game_server->second);
        }
    }

    return std::nullopt;
}

celeritas::service_registry_impl::optional_service_info celeritas::service_registry_impl::get_services_by_instance_id(const std::string& instance_id)
{
    std::lock_guard lock{ cleanup_timer_mutex_ };

    if (const auto iter = registry_.find(instance_id);
        iter != registry_.cend())
    {
        return iter->second;
    }

    return std::nullopt;
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
            remove_server(iter->second);
            iter = registry_.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}

void celeritas::service_registry_impl::remove_instance(const std::string& instance_id)
{
    std::lock_guard lock{ mutex_ };

    if (const auto iter = registry_.find(instance_id);
        iter != registry_.end())
    {
        remove_server(iter->second);
        registry_.erase(iter);
    }
}

celeritas::service_registry_impl::registry_type celeritas::service_registry_impl::get_services()
{
    std::lock_guard lock{ mutex_ };

    return registry_;
}

void celeritas::service_registry_impl::set_service_health(const std::string& instance_id, const health_check_level_type health_check_level)
{
    std::lock_guard lock{ mutex_ };

    if (const auto iter = registry_.find(instance_id);
        iter != registry_.end())
    {
        iter->second.set_health_check_level_type(health_check_level);
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

void celeritas::service_registry_impl::remove_server(const service_info& service_info)
{
    if (const auto service = server_.find(service_info.get_service_name());
        service != server_.cend())
    {
        if (const auto game_server = service->second.find(service_info.get_game_server_id());
            game_server != service->second.cend())
        {
            erase_if(game_server->second, [instance_id = service_info.get_instance_id()](const auto& element) {
                return element.get_instance_id() == instance_id;
            });

            if (game_server->second.empty())
            {
                service->second.erase(game_server);
            }

            if (service->second.empty())
            {
                server_.erase(service);
            }
        }
    }
}

celeritas::service_registry_impl::optional_service_info celeritas::service_registry_impl::get_idle_services(const service_info_container_type& service_info_container_type) const
{
    if (service_info_container_type.size() == 1)
    {
        if (const auto& service_info = service_info_container_type.at(0);
            service_info.get_health_check_level_type() == health_check_level_type::health)
        {
            return service_info;
        }
    }
    else
    {
        if (const auto& service_info = service_info_container_type.at(next_index_ % service_info_container_type.size());
            service_info.get_health_check_level_type() == health_check_level_type::health)
        {
            return service_info;
        }
        else
        {
            for (auto i = 0; i < service_info_container_type.size(); ++i)
            {
                if (const auto& element = service_info_container_type.at((next_index_ + i) % service_info_container_type.size());
                    element.get_health_check_level_type() == health_check_level_type::health)
                {
                    return service_info;
                }
            }
        }
    }

    return std::nullopt;
}
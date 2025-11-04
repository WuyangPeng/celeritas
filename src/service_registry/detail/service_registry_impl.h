#pragma once

#include "common/common_fwd.h"
#include "service_registry/service_info.h"

#include <boost/log/trivial.hpp>
#include <shared_mutex>
#include <unordered_map>

namespace celeritas
{
    class service_registry_impl : public std::enable_shared_from_this<service_registry_impl>
    {
    public:
        using class_type = service_registry_impl;
        using io_context_type = boost::asio::io_context;
        using service_info_container_type = std::vector<service_info>;
        using registry_type = std::unordered_map<std::string, service_info>;

        service_registry_impl() noexcept = default;

        ~service_registry_impl() noexcept = default;

        service_registry_impl(const service_registry_impl& rhs) = delete;

        service_registry_impl& operator=(const service_registry_impl& rhs) = delete;

        service_registry_impl(service_registry_impl&& rhs) noexcept = delete;

        service_registry_impl& operator=(service_registry_impl&& rhs) noexcept = delete;

        void register_service(const service_info& info);

        [[nodiscard]] service_info_container_type get_services(const std::string& service_name);

        void start_cleanup_timer(io_context_type& io_context);

        void cleanup_services_by_duration();

        void remove_instance(const std::string& instance_id);

        [[nodiscard]] registry_type get_services();

        void set_service_health(const std::string& instance_id, bool is_health);

    private:
        using registry_type_iterator = registry_type::iterator;
        using cleanup_timer_shared_ptr = std::shared_ptr<timer_base>;
        using seconds_type = std::chrono::seconds;
        using time_point_type = service_info::time_point_type;
        using severity_level_type = boost::log::trivial::severity_level;

        [[nodiscard]] static bool cleanup_service_entry(const registry_type_iterator& iter, const time_point_type& now);

        static void log_server_unresponsive(const registry_type_iterator& iter, int64_t duration, severity_level_type level, const std::string& description);

        registry_type registry_;
        std::shared_mutex mutex_;
        std::shared_mutex cleanup_timer_mutex_;
        cleanup_timer_shared_ptr cleanup_timer_;
    };
}

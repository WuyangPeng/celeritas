#pragma once

#include "service_registry/service_info.h"

#include <boost/asio/steady_timer.hpp>
#include <shared_mutex>
#include <unordered_map>

namespace celeritas
{
    class service_registry_impl : public std::enable_shared_from_this<service_registry_impl>
    {
    public:
        using class_type = service_registry_impl;
        using service_info_container_type = std::vector<service_info>;

        service_registry_impl() noexcept = default;

        ~service_registry_impl() noexcept = default;

        service_registry_impl(const service_registry_impl& rhs) = delete;

        service_registry_impl& operator=(const service_registry_impl& rhs) = delete;

        service_registry_impl(service_registry_impl&& rhs) noexcept = delete;

        service_registry_impl& operator=(service_registry_impl&& rhs) noexcept = delete;

        void register_service(const service_info& info);

        [[nodiscard]] service_info_container_type get_services(const std::string& service_name) const;

        void start_cleanup_timer(boost::asio::io_context& io_context);

    private:
        using registry_type = std::unordered_map<std::string, service_info>;
        using self_shared_ptr = std::shared_ptr<service_registry_impl>;
        using steady_timer_type = boost::asio::steady_timer;
        using steady_timer_unique_ptr = std::unique_ptr<steady_timer_type>;
        using registry_type_iterator = registry_type::iterator;

        void start_cleanup_timer(const self_shared_ptr& self) const;

        void cleanup_expired_services(const boost::system::error_code& error_code);

        void process_cleanup_logic();

        void cleanup_services_by_duration();

        [[nodiscard]] static bool cleanup_service_entry(const registry_type_iterator& iter, const service_info::time_point_type& now);

        registry_type registry_;
        mutable std::shared_mutex mutex_;
        steady_timer_unique_ptr cleanup_timer_interval_;
    };
}

#pragma once

#include "detail/service_registry_core_internal_fwd.h"
#include "service_registry/service_registry_fwd.h"

#include <boost/asio/io_context.hpp>
#include <vector>

namespace celeritas
{
    class service_registry
    {
    public:
        using class_type = service_registry;
        using io_context_type = boost::asio::io_context;
        using service_info_container_type = std::vector<service_info>;
        using registry_type = std::unordered_map<std::string, service_info>;
        using optional_service_info = std::optional<service_info>;
        using any_io_executor = boost::asio::any_io_executor;

        static void register_service(const service_info& info);

        static void clear_services(const std::string& service_name);

        [[nodiscard]] static service_info_container_type get_services(const std::string& service_name);

        [[nodiscard]] static service_info_container_type get_idle_services(const std::string& service_name);

        [[nodiscard]] static optional_service_info get_idle_services(const std::string& service_name, const std::string& game_server_id);

        [[nodiscard]] static optional_service_info get_services_by_instance_id(const std::string& instance_id);

        static void start_cleanup_timer(const any_io_executor& any_io_executor);

        static void remove_instance(const std::string& instance_id);

        static void set_service_health(const std::string& instance_id, health_check_level_type health_check_level);

        [[nodiscard]] static registry_type get_services();

    private:
        static service_registry_impl& get_service_registry_impl();
    };
}


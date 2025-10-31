#pragma once

#include "detail/service_registry_internal_fwd.h"
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

        static void register_service(const service_info& info);

        [[nodiscard]] static service_info_container_type get_services(const std::string& service_name);

        static void start_cleanup_timer(io_context_type& io_context);

        static void remove_instance(const std::string& instance_id);

    private:
        static service_registry_impl& get_service_registry_impl();
    };
}


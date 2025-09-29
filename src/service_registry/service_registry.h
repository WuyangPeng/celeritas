#pragma once

#include "service_info.h"
#include "boost/asio/io_context.hpp"
#include "detail/service_registry_internal_fwd.h"

#include <vector>

namespace celeritas
{
    class service_registry
    {
    public:
        using class_type = service_registry;
        using service_info_container_type = std::vector<service_info>;
        using io_context_type = boost::asio::io_context;

        static void register_service(const service_info& info);

        [[nodiscard]] static service_info_container_type get_services(const std::string& service_name);

        static void start_cleanup_timer(io_context_type& io_context);

    private:
        static service_registry_impl& get_service_registry_impl();
    };
}


#pragma once

#include "config/config_fwd.h"
#include "network/network_fwd.h"

#include <boost/asio.hpp>

namespace celeritas
{
    class service_registry_loader
    {
    public:
        using class_type = service_registry_loader;

        using io_context_type = boost::asio::io_context;
        using network_message_callback_weak_ptr = std::weak_ptr<network_message_callback>;
        using tcp_client_shared_ptr = std::shared_ptr<tcp_client>;
        using any_io_executor = boost::asio::any_io_executor;

        static tcp_client_shared_ptr loader_service_registry(const any_io_executor& any_io_executor,
                                                             const service_registry_config& service_registry_config,
                                                             const network_message_callback_weak_ptr& network_message_callback,
                                                             const std::string& game_server_id,
                                                             const std::string& server_type);
    };
}


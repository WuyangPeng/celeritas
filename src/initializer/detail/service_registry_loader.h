#pragma once

#include "config/service_registry_config.h"
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

        static tcp_client_shared_ptr loader_service_registry(io_context_type& io_context,
                                                             const service_registry_config& service_registry_config,
                                                             const network_message_callback_weak_ptr& network_message_callback,
                                                             const std::string& game_server_id,
                                                             const std::string& server_type);
    };
}


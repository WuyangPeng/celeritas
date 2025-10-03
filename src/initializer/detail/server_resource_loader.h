#pragma once

#include "config/server_config.h"
#include "network/listener.h"

#include <boost/asio/io_context.hpp>

namespace celeritas
{
    class server_resource_loader
    {
    public:
        using class_type = server_resource_loader;
        using io_context_type = boost::asio::io_context;
        using network_message_callback_weak_ptr = std::weak_ptr<network_message_callback>;
        using listener_shared_ptr = std::shared_ptr<listener>;

        static listener_shared_ptr loader_server(io_context_type& io_context, const server_config& server_config, const server_network_config& server_network_config, const network_message_callback_weak_ptr& network_message_callback);
    };
}
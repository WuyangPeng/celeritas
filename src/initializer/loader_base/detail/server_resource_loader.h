#pragma once

#include "config/aggregate/server_config.h"
#include "network/core/listener.h"

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
        using any_io_executor = boost::asio::any_io_executor;

        static listener_shared_ptr loader_server(const any_io_executor& any_io_executor, const server_config& server_config, const server_network_config& server_network_config, const network_message_callback_weak_ptr& network_message_callback);
    };
}
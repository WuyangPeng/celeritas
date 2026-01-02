#pragma once

#include "config/local/database_config.h"

#include <boost/asio/io_context.hpp>

namespace celeritas
{
    class database_resource_loader
    {
    public:
        using class_type = database_resource_loader;
        using io_context_type = boost::asio::io_context;

        static void loader_database(io_context_type& io_context, const database_config& database_config);
    };
}
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
        using any_io_executor = boost::asio::any_io_executor;

        static void loader_database(const any_io_executor& any_io_executor, const database_config& database_config);
    };
}
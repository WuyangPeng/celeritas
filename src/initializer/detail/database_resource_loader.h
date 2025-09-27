#pragma once

#include "config/database_config.h"

#include <boost/asio/io_context.hpp>

namespace celeritas
{
    class database_resource_loader
    {
    public:
        using class_type = database_resource_loader;

        static void loader_config(boost::asio::io_context& io_context, const database_config& database_config);
    };
}
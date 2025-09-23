#pragma once
#include "database_type.h"

#include <string>

namespace celeritas
{
    class database_config
    {
    public:
        using class_type = database_config;

    private:
        database_type database_type_ = database_type::unknown;
        std::string host_;
        int port_ = 0;
        std::string user_;
        std::string password_;
        std::string db_name_;
        int max_connections_ = 10;
        long timeout_seconds_ = 5;
    };
}

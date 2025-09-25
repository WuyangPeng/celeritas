#pragma once
#include "database_type.h"

#include <string>

namespace celeritas
{
    class database_config
    {
    public:
        using class_type = database_config;

        database_config() noexcept = default;

        database_config(std::string name,
                        database_type database_type,
                        std::string host,
                        int port,
                        std::string user,
                        std::string password,
                        std::string db_name,
                        int max_connections,
                        int timeout_seconds);

        [[nodiscard]] std::string get_name() const;

    private:
        std::string name_;
        database_type database_type_ = database_type::unknown;
        std::string host_;
        int port_ = 0;
        std::string user_;
        std::string password_;
        std::string db_name_;
        int max_connections_ = 10;
        int timeout_seconds_ = 5;
    };
}

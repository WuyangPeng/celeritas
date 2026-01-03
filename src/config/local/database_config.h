#pragma once

#include "config/basic/database_type.h"
#include "config/config_constant.h"

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
                        int min_connections,
                        int max_connections,
                        int timeout_seconds,
                        int expire_seconds) noexcept;

        [[nodiscard]] std::string get_name() const;

        [[nodiscard]] database_type get_database_type() const noexcept;

        [[nodiscard]] std::string get_host() const;

        [[nodiscard]] int get_port() const noexcept;

        [[nodiscard]] std::string get_user() const;

        [[nodiscard]] std::string get_password() const;

        [[nodiscard]] std::string get_db_name() const;

        [[nodiscard]] int get_min_connections() const noexcept;

        [[nodiscard]] int get_max_connections() const noexcept;

        [[nodiscard]] int get_timeout_seconds() const noexcept;

        [[nodiscard]] int get_expire_seconds() const noexcept;

    private:
        std::string name_;
        database_type database_type_ = database_type::unknown;
        std::string host_;
        int port_ = 0;
        std::string user_;
        std::string password_;
        std::string db_name_;
        int min_connections_ = default_database_min_connections;
        int max_connections_ = default_database_max_connections;
        int timeout_seconds_ = default_database_timeout_seconds;
        int expire_seconds_ = default_database_expire_seconds;
    };
}

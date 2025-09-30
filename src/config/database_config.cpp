#include "database_config.h"

#include <utility>

celeritas::database_config::database_config(std::string name, database_type database_type, std::string host, int port, std::string user, std::string password, std::string db_name, const int min_connections, const int max_connections, const int timeout_seconds)
    : name_{ std::move(name) }, database_type_{ database_type }, host_{ std::move(host) }, port_{ port }, user_{ std::move(user) }, password_{ std::move(password) }, db_name_{ std::move(db_name) }, min_connections_{ min_connections }, max_connections_{ max_connections }, timeout_seconds_{ timeout_seconds }
{
}

std::string celeritas::database_config::get_name() const
{
    return name_;
}

celeritas::database_type celeritas::database_config::get_database_type() const
{
    return database_type_;
}

std::string celeritas::database_config::get_host() const
{
    return host_;
}

int celeritas::database_config::get_port() const
{
    return port_;
}

std::string celeritas::database_config::get_user() const
{
    return user_;
}

std::string celeritas::database_config::get_password() const
{
    return password_;
}

std::string celeritas::database_config::get_db_name() const
{
    return db_name_;
}

int celeritas::database_config::get_min_connections() const
{
    return min_connections_;
}

int celeritas::database_config::get_max_connections() const
{
    return max_connections_;
}

int celeritas::database_config::get_timeout_seconds() const
{
    return timeout_seconds_;
}
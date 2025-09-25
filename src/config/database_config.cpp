#include "database_config.h"

celeritas::database_config::database_config(std::string name, database_type database_type, std::string host, int port, std::string user, std::string password, std::string db_name, int max_connections, int timeout_seconds)
    : name_{ name }, database_type_{ database_type }, host_{ host }, port_{ port }, user_{ user }, password_{ password }, db_name_{ db_name }, max_connections_{ max_connections }, timeout_seconds_{ timeout_seconds }

{
}

std::string celeritas::database_config::get_name() const
{
    return name_;
}
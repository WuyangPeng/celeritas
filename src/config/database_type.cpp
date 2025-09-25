#include "database_type.h"

celeritas::database_type celeritas::get_database_type(const std::string& database_name)
{
    if (database_name == "mysql")
        return database_type::mysql;

    if (database_name == "mongo")
        return database_type::mongo;

    if (database_name == "redis")
        return database_type::redis;

    return database_type::unknown;
}
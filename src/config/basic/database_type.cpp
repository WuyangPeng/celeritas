#include "database_type.h"
#include "common/core/celeritas_error.h"

#include <map>

celeritas::database_type celeritas::get_database_type(const std::string& database_name)
{
    using database_container_type = std::map<std::string, database_type>;

    static database_container_type database{ { "mysql", database_type::mysql },
                                             { "mongo", database_type::mongo },
                                             { "redis", database_type::redis }, };

    if (const auto iter = database.find(database_name);
        iter != database.end())
    {
        return iter->second;
    }

    throw celeritas_error{ "database_type error,name = {}", database_name };
}
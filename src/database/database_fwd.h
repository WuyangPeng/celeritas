#pragma once

namespace celeritas
{
    template <typename SessionType>
    class connection_pool_base;

    class database_pool_manager;
    class database_pool;
    class database_session;
    class mysql_database_session;
    class mongo_database_session;
    class redis_database_session;
}

#include "connection_pool_base.tpp"
#include "database_pool_manager.h"
#include "mongo_database_session.h"
#include "mysql_database_session.h"
#include "redis_database_session.h"
#include "common/celeritas_error.h"

celeritas::database_pool_manager& celeritas::database_pool_manager::get_instance()
{
    static database_pool_manager manager{};

    return manager;
}

celeritas::database_pool_manager::database_pool_shared_ptr celeritas::database_pool_manager::create_pool(const std::string& name,
                                                                                                         database_type database_type,
                                                                                                         io_context_type& io_context,
                                                                                                         const std::string& host,
                                                                                                         const int port,
                                                                                                         const std::string& user,
                                                                                                         const std::string& password,
                                                                                                         const std::string& db_name,
                                                                                                         int min_connections,
                                                                                                         int max_connections)
{
    switch (database_type)
    {
        case database_type::mysql:
        {
            return create_mysql_pool(name, io_context, host, port, user, password, db_name, min_connections, max_connections);
        }
        case database_type::mongo:
        {
            return create_mongo_pool(name, io_context, host, port, user, password, db_name, min_connections, max_connections);
        }
        case database_type::redis:
        {
            return create_redis_pool(name, io_context, host, port, user, password, db_name, min_connections, max_connections);
        }
        default:
        {
            throw celeritas_error("create pool ,name = " + name + ",database_type =" + std::to_string(static_cast<int>(database_type)) + " is  not exist.");
        }
    }
}

celeritas::database_pool_manager::database_pool_shared_ptr celeritas::database_pool_manager::get_pool(const std::string& name)
{
    std::unique_lock lock{ mutex_ };

    if (const auto pool = pools_.find(name);
        pool != pools_.cend())
    {
        return pool->second;
    }

    throw celeritas_error("get pool ,name = " + name + " is  not exist.");
}

celeritas::database_pool_manager::database_pool_shared_ptr celeritas::database_pool_manager::create_mysql_pool(const std::string& name,
                                                                                                               io_context_type& io_context,
                                                                                                               const std::string& host,
                                                                                                               int port,
                                                                                                               const std::string& user,
                                                                                                               const std::string& password,
                                                                                                               const std::string& db_name,
                                                                                                               int min_connections,
                                                                                                               int max_connections)
{
    std::unique_lock lock{ mutex_ };

    auto pool = std::make_shared<connection_pool_base<mysql_database_session> >(io_context, host, port, user, password, db_name, min_connections, max_connections);

    pools_.insert({ name, pool });

    return pool;
}

celeritas::database_pool_manager::database_pool_shared_ptr celeritas::database_pool_manager::create_mongo_pool(const std::string& name, io_context_type& io_context, const std::string& host, int port, const std::string& user, const std::string& password, const std::string& db_name, int min_connections, int max_connections)
{
    if (!mongo_instance_)
    {
        mongo_instance_ = std::make_unique<mongocxx::instance>();
    }

    const auto url = "mongodb://" + user + ":" + password + "@" + host + ":" + std::to_string(port) + "/" + db_name;

    std::unique_lock lock{ mutex_ };

    auto pool = std::make_shared<connection_pool_base<mongo_database_session> >(io_context, url, db_name, min_connections, max_connections);

    pools_.insert({ name, pool });

    return pool;
}

celeritas::database_pool_manager::database_pool_shared_ptr celeritas::database_pool_manager::create_redis_pool(const std::string& name, io_context_type& io_context, const std::string& host, int port, const std::string& user, const std::string& password, const std::string& db_name, int min_connections, int max_connections)
{
    std::unique_lock lock{ mutex_ };

    auto pool = std::make_shared<connection_pool_base<redis_database_session> >(io_context, host, port, user, password, min_connections, max_connections);

    pools_.insert({ name, pool });

    return pool;
}

void celeritas::database_pool_manager::start_cleanup_timer(io_context_type& io_context)
{
    std::unique_lock lock{ mutex_ };

    for (const auto& pool : pools_)
    {
        pool.second->start_cleanup_timer(io_context);
    }
}
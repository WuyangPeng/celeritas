#include "connection_pool_base.tpp"
#include "database_pool_manager.h"
#include "mongo_database_session.h"
#include "mysql_database_session.h"
#include "redis_database_session.h"
#include "common/celeritas_error.h"

#include <ranges>

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
                                                                                                         const int min_connections,
                                                                                                         const int max_connections)
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
    std::lock_guard lock{ mutex_ };

    if (const auto pool = pools_.find(name);
        pool != pools_.cend())
    {
        return pool->second;
    }

    throw celeritas_error("get pool ,name = " + name + " is  not exist.");
}

void celeritas::database_pool_manager::start_cleanup_timer(io_context_type& io_context)
{
    std::lock_guard lock{ mutex_ };

    for (const auto& pool : pools_ | std::views::values)
    {
        pool->start_cleanup_timer(io_context);
    }
}

void celeritas::database_pool_manager::release_pool()
{
    std::lock_guard lock{ mutex_ };

    for (const auto& pool : pools_ | std::views::values)
    {
        pool->stop_cleanup_timer();
    }

    pools_.clear();
}

bool celeritas::database_pool_manager::is_health()
{
    std::lock_guard lock{ mutex_ };

    for (const auto& pool : pools_ | std::views::values)
    {
        if (!pool->is_health())
        {
            return false;
        }
    }

    return true;
}

celeritas::database_pool_manager::database_pool_shared_ptr celeritas::database_pool_manager::create_mysql_pool(const std::string& name,
                                                                                                               io_context_type& io_context,
                                                                                                               const std::string& host,
                                                                                                               const int port,
                                                                                                               const std::string& user,
                                                                                                               const std::string& password,
                                                                                                               const std::string& db_name,
                                                                                                               const int min_connections,
                                                                                                               const int max_connections)
{
    auto pool = std::make_shared<connection_pool_base<mysql_database_session> >(io_context, host, port, user, password, db_name, min_connections, max_connections);

    std::lock_guard lock{ mutex_ };

    pools_.insert({ name, pool });

    return pool;
}

celeritas::database_pool_manager::database_pool_shared_ptr celeritas::database_pool_manager::create_mongo_pool(const std::string& name,
                                                                                                               io_context_type& io_context,
                                                                                                               const std::string& host,
                                                                                                               const int port,
                                                                                                               const std::string& user,
                                                                                                               const std::string& password,
                                                                                                               const std::string& db_name,
                                                                                                               const int min_connections,
                                                                                                               const int max_connections)
{
    static auto mongo_instance = std::make_unique<mongocxx::instance>();

    const auto url = "mongodb://" + user + ":" + password + "@" + host + ":" + std::to_string(port) + "/" + db_name;

    auto pool = std::make_shared<connection_pool_base<mongo_database_session> >(io_context, url, db_name, min_connections, max_connections);

    std::lock_guard lock{ mutex_ };

    pools_.insert({ name, pool });

    return pool;
}

celeritas::database_pool_manager::database_pool_shared_ptr celeritas::database_pool_manager::create_redis_pool(const std::string& name,
                                                                                                               io_context_type& io_context,
                                                                                                               const std::string& host,
                                                                                                               const int port,
                                                                                                               const std::string& user,
                                                                                                               const std::string& password,
                                                                                                               const std::string& db_name,
                                                                                                               const int min_connections,
                                                                                                               const int max_connections)
{
    auto pool = std::make_shared<connection_pool_base<redis_database_session> >(io_context, host, port, user, password, min_connections, max_connections);

    std::lock_guard lock{ mutex_ };

    pools_.insert({ name, pool });

    return pool;
}
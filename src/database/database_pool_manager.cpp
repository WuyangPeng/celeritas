#include "connection_pool_base.tpp"
#include "database_pool_manager.h"
#include "mongo_database_session.h"
#include "mysql_database_session.h"
#include "redis_database_session.h"
#include "common/celeritas_error.h"
#include "basis_database_manager.h"

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
                                                                                                         const int max_connections,
                                                                                                         const int expire_seconds)
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
            return create_redis_pool(name, io_context, host, port, user, password, db_name, min_connections, max_connections, expire_seconds);
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

celeritas::database_pool_manager::bool_awaitable_type celeritas::database_pool_manager::is_health()
{
    std::lock_guard lock{ mutex_ };

    for (const auto& pool : pools_ | std::views::values)
    {
        if (const auto result = co_await pool->is_health();
            !result)
        {
            co_return false;
        }
    }

    co_return true;
}

void celeritas::database_pool_manager::execute_changes(io_context_type& io_context, const std::string& name, const basis_database_manager_const_shared_ptr& database)
{
    const auto pool = get_pool(name);

    boost::asio::co_spawn(io_context,
                          [pool,database] {
                              return pool->execute_changes(database);
                          }, boost::asio::detached);
}

celeritas::database_pool_manager::basis_database_manager_awaitable_type celeritas::database_pool_manager::select_one(const std::string& name, const basis_database_manager_const_shared_ptr& database, const database_field_container& field_name_container)
{
    const auto pool = get_pool(name);

    co_return co_await pool->select_one(database, field_name_container);
}

celeritas::database_pool_manager::result_container_awaitable_type celeritas::database_pool_manager::select_all(const std::string& name, const basis_database_manager_const_shared_ptr& database, const database_field_container& field_name_container)
{
    const auto pool = get_pool(name);

    co_return co_await pool->select_all(database, field_name_container);
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
                                                                                                               const int max_connections,
                                                                                                               const int expire_seconds)
{
    auto pool = std::make_shared<connection_pool_base<redis_database_session> >(io_context, host, port, user, password, db_name, min_connections, max_connections, expire_seconds);

    std::lock_guard lock{ mutex_ };

    pools_.insert({ name, pool });

    return pool;
}
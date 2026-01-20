#include "connection_pool.tpp"
#include "database_pool_manager.h"
#include "common/core/celeritas_error.h"
#include "common/core/enum_cast.h"
#include "database/session/mongo_database_session.h"
#include "database/session/mysql_database_session.h"
#include "database/session/redis_database_session.h"

#include <mongocxx/instance.hpp>

#include <ranges>

celeritas::database_pool_manager& celeritas::database_pool_manager::get_instance()
{
    static database_pool_manager manager{};

    return manager;
}

celeritas::database_pool_manager::database_pool_shared_ptr celeritas::database_pool_manager::create_pool(const std::string& name,
                                                                                                         const database_type database_type,
                                                                                                         const any_io_executor& any_io_executor,
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
            return create_mysql_pool(name, any_io_executor, host, port, user, password, db_name, min_connections, max_connections);
        }
        case database_type::mongo:
        {
            return create_mongo_pool(name, any_io_executor, host, port, user, password, db_name, min_connections, max_connections);
        }
        case database_type::redis:
        {
            return create_redis_pool(name, any_io_executor, host, port, user, password, db_name, min_connections, max_connections, expire_seconds);
        }
        default:
        {
            throw celeritas_error{ "create pool ,name = {}, database_type ={} is not exist.", name, enum_cast_underlying(database_type) };
        }
    }
}

celeritas::database_pool_manager::database_pool_shared_ptr celeritas::database_pool_manager::get_pool(const std::string& name)
{
    std::shared_lock lock{ mutex_ };

    if (mock_pool_ != nullptr)
    {
        return mock_pool_;
    }

    if (const auto pool = pools_.find(name);
        pool != pools_.cend())
    {
        return pool->second;
    }

    throw celeritas_error{ "get pool name = {} is  not exist.", name };
}

void celeritas::database_pool_manager::start_cleanup_timer(const any_io_executor& any_io_executor)
{
    std::lock_guard lock{ mutex_ };

    for (const auto& pool : pools_ | std::views::values)
    {
        pool->start_cleanup_timer(any_io_executor);
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

    if (mock_pool_ != nullptr)
    {
        mock_pool_->stop_cleanup_timer();
        mock_pool_ = nullptr;
    }
}

celeritas::database_pool_manager::bool_awaitable_type celeritas::database_pool_manager::is_health()
{
    std::shared_lock lock{ mutex_ };

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

void celeritas::database_pool_manager::set_mock_pool(const database_pool_shared_ptr& mock_pool)
{
    std::lock_guard lock{ mutex_ };
    mock_pool_ = mock_pool;
}

void celeritas::database_pool_manager::create_mongo_instance()
{
    static auto mongo_instance = std::make_unique<mongocxx::instance>();
}

celeritas::database_pool_manager::database_pool_shared_ptr celeritas::database_pool_manager::create_mysql_pool(const std::string& name,
                                                                                                               const any_io_executor& any_io_executor,
                                                                                                               const std::string& host,
                                                                                                               const int port,
                                                                                                               const std::string& user,
                                                                                                               const std::string& password,
                                                                                                               const std::string& db_name,
                                                                                                               const int min_connections,
                                                                                                               const int max_connections)
{
    auto pool = std::make_shared<connection_pool<mysql_database_session> >(any_io_executor, host, port, user, password, db_name, min_connections, max_connections);

    std::lock_guard lock{ mutex_ };

    pools_.emplace(name, pool);

    return pool;
}

celeritas::database_pool_manager::database_pool_shared_ptr celeritas::database_pool_manager::create_mongo_pool(const std::string& name,
                                                                                                               const any_io_executor& any_io_executor,
                                                                                                               const std::string& host,
                                                                                                               const int port,
                                                                                                               const std::string& user,
                                                                                                               const std::string& password,
                                                                                                               const std::string& db_name,
                                                                                                               const int min_connections,
                                                                                                               const int max_connections)
{
    create_mongo_instance();

    const auto url = "mongodb://" + user + ":" + password + "@" + host + ":" + std::to_string(port) + "/" + db_name;

    auto pool = std::make_shared<connection_pool<mongo_database_session> >(any_io_executor, url, db_name, min_connections, max_connections);

    std::lock_guard lock{ mutex_ };

    pools_.emplace(name, pool);

    return pool;
}

celeritas::database_pool_manager::database_pool_shared_ptr celeritas::database_pool_manager::create_redis_pool(const std::string& name,
                                                                                                               const any_io_executor& any_io_executor,
                                                                                                               const std::string& host,
                                                                                                               const int port,
                                                                                                               const std::string& user,
                                                                                                               const std::string& password,
                                                                                                               const std::string& db_name,
                                                                                                               const int min_connections,
                                                                                                               const int max_connections,
                                                                                                               const int expire_seconds)
{
    auto pool = std::make_shared<connection_pool<redis_database_session> >(any_io_executor, host, port, user, password, db_name, min_connections, max_connections, expire_seconds);

    std::lock_guard lock{ mutex_ };

    pools_.insert({ name, pool });

    return pool;
}
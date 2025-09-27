#include "database_pool_manager.h"
#include "common/celeritas_error.h"

celeritas::database_pool_manager& celeritas::database_pool_manager::get_instance()
{
    static database_pool_manager manager{};

    return manager;
}

celeritas::database_pool_manager::database_pool_shared_ptr celeritas::database_pool_manager::create_pool(const std::string& name,
                                                                                                         database_type database_type,
                                                                                                         boost::asio::io_context& io_context,
                                                                                                         const std::string& host,
                                                                                                         const uint16_t port,
                                                                                                         const std::string& user,
                                                                                                         const std::string& password,
                                                                                                         const std::string& db_name,
                                                                                                         const size_t pool_size)
{
    switch (database_type)
    {
        case database_type::mysql:
        {
            return create_mysql_pool(name, io_context, host, port, user, password, db_name, pool_size);
        }
        default:
        {
            throw new celeritas_error("create pool ,name = " + name + ",database_type =" + std::to_string(static_cast<int>(database_type)) + " is  not exist.");
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

    throw new celeritas_error("get pool ,name = " + name + " is  not exist.");
}

celeritas::database_pool_manager::database_pool_shared_ptr celeritas::database_pool_manager::create_mysql_pool(const std::string& name,
                                                                                                               boost::asio::io_context& io_context,
                                                                                                               const std::string& host,
                                                                                                               uint16_t port,
                                                                                                               const std::string& user,
                                                                                                               const std::string& password,
                                                                                                               const std::string& db_name,
                                                                                                               size_t pool_size)
{
    std::lock_guard lock{ mutex_ };

    database_pool_shared_ptr pool = std::make_shared<mysql_database_pool>(io_context, host, port, user, password, db_name, pool_size);

    pools_.insert({ name, pool });

    return pool;
}
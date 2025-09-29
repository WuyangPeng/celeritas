#pragma once

#include "mysql_database_pool.h"
#include "config/database_type.h"

#include <map>

namespace celeritas
{
    class database_pool_manager
    {
    public:
        using class_type = database_pool_manager;

        using database_pool_shared_ptr = std::shared_ptr<database_pool>;

        [[nodiscard]] static database_pool_manager& get_instance();

        [[nodiscard]] database_pool_shared_ptr create_pool(const std::string& name,
                                                           database_type database_type,
                                                           boost::asio::io_context& io_context,
                                                           const std::string& host,
                                                           uint16_t port,
                                                           const std::string& user,
                                                           const std::string& password,
                                                           const std::string& db_name,
                                                           int min_connections,
                                                           int max_connections);

        [[nodiscard]] database_pool_shared_ptr get_pool(const std::string& name);

    private:
        using database_pool_container = std::map<std::string, database_pool_shared_ptr>;

        database_pool_manager() noexcept = default;

        [[nodiscard]] database_pool_shared_ptr create_mysql_pool(const std::string& name,
                                                                 boost::asio::io_context& io_context,
                                                                 const std::string& host,
                                                                 uint16_t port,
                                                                 const std::string& user,
                                                                 const std::string& password,
                                                                 const std::string& db_name,
                                                                 int min_connections,
                                                                 int max_connections);

        [[nodiscard]] database_pool_shared_ptr create_mongo_pool(const std::string& name,
                                                                 boost::asio::io_context& io_context,
                                                                 const std::string& host,
                                                                 uint16_t port,
                                                                 const std::string& user,
                                                                 const std::string& password,
                                                                 const std::string& db_name,
                                                                 size_t pool_size);

        [[nodiscard]] database_pool_shared_ptr create_redis_pool(const std::string& name,
                                                                 boost::asio::io_context& io_context,
                                                                 const std::string& host,
                                                                 uint16_t port,
                                                                 const std::string& user,
                                                                 const std::string& password,
                                                                 const std::string& db_name,
                                                                 size_t pool_size);

        database_pool_container pools_;
        std::mutex mutex_;
    };
}

#pragma once

#include "database_pool_base.h"
#include "config/database_type.h"

#include <mongocxx/instance.hpp>

#include <map>
#include <shared_mutex>

namespace celeritas
{
    class database_pool_manager
    {
    public:
        using class_type = database_pool_manager;
        using io_context_type = boost::asio::io_context;
        using database_pool_shared_ptr = std::shared_ptr<database_pool_base>;
        using bool_awaitable_type = boost::asio::awaitable<bool>;
        using result_container = std::vector<database_entity_change>;
        using database_field_container = std::vector<database_field>;
        using optional_database_entity_change = std::optional<database_entity_change>;
        using database_entity_change_awaitable_type = boost::asio::awaitable<optional_database_entity_change>;
        using result_container_awaitable_type = boost::asio::awaitable<result_container>;
        using database_entity_change_const_shared_ptr = std::shared_ptr<const database_entity_change>;

        [[nodiscard]] static database_pool_manager& get_instance();

        [[nodiscard]] database_pool_shared_ptr create_pool(const std::string& name,
                                                           database_type database_type,
                                                           io_context_type& io_context,
                                                           const std::string& host,
                                                           int port,
                                                           const std::string& user,
                                                           const std::string& password,
                                                           const std::string& db_name,
                                                           int min_connections,
                                                           int max_connections,
                                                           int expire_seconds);

        [[nodiscard]] database_pool_shared_ptr get_pool(const std::string& name);

        void start_cleanup_timer(io_context_type& io_context);

        void release_pool();

        [[nodiscard]] bool_awaitable_type is_health();

    private:
        using database_pool_container = std::map<std::string, database_pool_shared_ptr>;

        database_pool_manager() noexcept = default;

        [[nodiscard]] database_pool_shared_ptr create_mysql_pool(const std::string& name,
                                                                 io_context_type& io_context,
                                                                 const std::string& host,
                                                                 int port,
                                                                 const std::string& user,
                                                                 const std::string& password,
                                                                 const std::string& db_name,
                                                                 int min_connections,
                                                                 int max_connections);

        [[nodiscard]] database_pool_shared_ptr create_mongo_pool(const std::string& name,
                                                                 io_context_type& io_context,
                                                                 const std::string& host,
                                                                 int port,
                                                                 const std::string& user,
                                                                 const std::string& password,
                                                                 const std::string& db_name,
                                                                 int min_connections,
                                                                 int max_connections);

        [[nodiscard]] database_pool_shared_ptr create_redis_pool(const std::string& name,
                                                                 io_context_type& io_context,
                                                                 const std::string& host,
                                                                 int port,
                                                                 const std::string& user,
                                                                 const std::string& password,
                                                                 const std::string& db_name,
                                                                 int min_connections,
                                                                 int max_connections,
                                                                 int expire_seconds);

        database_pool_container pools_;
        std::shared_mutex mutex_;
    };
}
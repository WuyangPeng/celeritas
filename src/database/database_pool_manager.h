#pragma once

#include "database_pool.h"
#include "config/database_type.h"

#include <mongocxx/instance.hpp>

#include <map>

namespace celeritas
{
    class database_pool_manager
    {
    public:
        using class_type = database_pool_manager;
        using io_context_type = boost::asio::io_context;
        using database_pool_shared_ptr = std::shared_ptr<database_pool>;
        using bool_awaitable_type = boost::asio::awaitable<bool>;
        using result_container = std::vector<basis_database_manager>;
        using database_field_container = std::vector<database_field>;
        using optional_basis_database_manager = std::optional<basis_database_manager>;
        using basis_database_manager_awaitable_type = boost::asio::awaitable<optional_basis_database_manager>;
        using result_container_awaitable_type = boost::asio::awaitable<result_container>;
        using basis_database_manager_const_shared_ptr = std::shared_ptr<const basis_database_manager>;

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

        void execute_changes(io_context_type& io_context, const std::string& name, const basis_database_manager_const_shared_ptr& database);

        [[nodiscard]] basis_database_manager_awaitable_type select_one(const std::string& name, const basis_database_manager_const_shared_ptr& database, const database_field_container& field_name_container);

        [[nodiscard]] result_container_awaitable_type select_all(const std::string& name, const basis_database_manager_const_shared_ptr& database, const database_field_container& field_name_container);

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
        std::mutex mutex_;
    };
}

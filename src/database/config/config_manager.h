#pragma once

#include "database/generated/mysql/config/time_refresh.h"

#include <boost/asio.hpp>

#include <shared_mutex>

namespace celeritas
{
    class config_manager
    {
    public:
        using class_type = config_manager;
        using io_context_type = boost::asio::io_context;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using optional_time_refresh = std::optional<time_refresh>;
        using any_io_executor = boost::asio::any_io_executor;

        [[nodiscard]] static config_manager& get_instance();

        void reload_from_db(const any_io_executor& any_io_executor, const std::string& db_name, int64_t id);

        void load_from_db(const any_io_executor& any_io_executor);

        [[nodiscard]] optional_time_refresh get_time_refresh(int64_t id);

    private:
        using time_refresh_container_type = std::unordered_map<int64_t, time_refresh>;
        using database_pool_shared_ptr = std::shared_ptr<database_pool_base>;

        config_manager() noexcept = default;

        [[nodiscard]] void_awaitable_type load_from_db();

        [[nodiscard]] void_awaitable_type load_time_refresh_db(const database_pool_shared_ptr& mysql_pool);

        [[nodiscard]] void_awaitable_type load_from_db(const std::string& db_name, int64_t id);

        [[nodiscard]] void_awaitable_type load_time_refresh_db(const database_pool_shared_ptr& mysql_pool, int64_t id);

        std::shared_mutex shared_mutex_;
        time_refresh_container_type time_refresh_;
    };
}
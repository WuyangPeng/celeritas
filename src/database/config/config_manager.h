#pragma once

#include "config_table.h"
#include "config_table_base.h"
#include "database/generated/mysql/config/time_refresh.h"

#include <boost/asio.hpp>

#include <memory>
#include <unordered_map>

namespace celeritas
{
    class config_manager
    {
    public:
        using class_type = config_manager;
        using any_io_executor = boost::asio::any_io_executor;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using const_time_refresh_shared_ptr = std::shared_ptr<const time_refresh>;
        using optional_const_time_refresh_shared_ptr = std::optional<const_time_refresh_shared_ptr>;

        [[nodiscard]] static config_manager& get_instance();

        void reload_from_db(const any_io_executor& any_io_executor, const std::string& db_name, int64_t id);

        void load_from_db(const any_io_executor& any_io_executor);

        [[nodiscard]] optional_const_time_refresh_shared_ptr get_time_refresh(int64_t id);

    private:
        using database_pool_shared_ptr = std::shared_ptr<database_pool_base>;
        using config_table_unique_ptr = std::unique_ptr<config_table_base>;
        using config_table_container = std::unordered_map<std::string_view, config_table_unique_ptr>;
        using time_refresh_table = config_table<time_refresh>;

        config_manager();

        void register_config_tables();

        void register_time_refresh_table();

        [[nodiscard]] void_awaitable_type load_from_db();

        [[nodiscard]] void_awaitable_type load_from_db(const std::string& db_name, int64_t id);

        config_table_container config_tables_;
        std::shared_mutex shared_mutex_;
    };
}

#include "config_manager.h"
#include "config_table.tpp"
#include "common/core/celeritas_error.h"
#include "common/core/noexcept_safe_call_and_log.h"
#include "common/logging/logger.h"
#include "database/database_constant.h"
#include "database/pool/database_pool_manager.h"

#include <boost/polymorphic_cast.hpp>

#include <ranges>

celeritas::config_manager& celeritas::config_manager::get_instance()
{
    static config_manager instance{};

    return instance;
}

void celeritas::config_manager::reload_from_db(const any_io_executor& any_io_executor, const std::string& db_name, int64_t id)
{
    if (db_name.empty() && id == 0)
    {
        return load_from_db(any_io_executor);
    }

    boost::asio::co_spawn(any_io_executor,
                          noexcept_safe_call_and_log_awaitable([db_name, id] {
                                                                   return get_instance().load_from_db(db_name, id);
                                                               },
                                                               database_channel,
                                                               "load config from db error: "),
                          boost::asio::detached);
}

void celeritas::config_manager::load_from_db(const any_io_executor& any_io_executor)
{
    boost::asio::co_spawn(any_io_executor,
                          noexcept_safe_call_and_log_awaitable([] {
                                                                   return get_instance().load_from_db();
                                                               },
                                                               database_channel,
                                                               "load config from db error: "),
                          boost::asio::detached);
}

celeritas::config_manager::optional_const_time_refresh_shared_ptr celeritas::config_manager::get_time_refresh(const int64_t id)
{
    if (const auto iter = config_tables_.find(time_refresh_db_name);
        iter != config_tables_.cend())
    {
        const auto table = boost::polymorphic_downcast<time_refresh_table*>(iter->second.get());
        return table->get_item(id);
    }

    return std::nullopt;
}

void celeritas::config_manager::clear()
{
    for (const auto& table : config_tables_ | std::views::values)
    {
        table->clear();
    }
}

celeritas::config_manager::config_manager()
    : config_tables_{}
{
    register_config_tables();
}

void celeritas::config_manager::register_config_tables()
{
    register_time_refresh_table();
}

void celeritas::config_manager::register_time_refresh_table()
{
    auto table = std::make_unique<time_refresh_table>(time_refresh_db_name);
    config_tables_.emplace(table->get_name(), std::move(table));
}

celeritas::config_manager::void_awaitable_type celeritas::config_manager::load_from_db()
{
    const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_config_db_name.data());
    if (!mysql_pool)
    {
        co_return;
    }

    for (const auto& table : config_tables_ | std::views::values)
    {
        co_await table->load_all(mysql_pool);
    }
}

celeritas::config_manager::void_awaitable_type celeritas::config_manager::load_from_db(const std::string& db_name, const int64_t id)
{
    const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_config_db_name.data());
    if (!mysql_pool)
    {
        co_return;
    }

    if (const auto iter = config_tables_.find(db_name);
        iter != config_tables_.cend())
    {
        if (id == 0)
        {
            co_await iter->second->load_all(mysql_pool);
        }
        else
        {
            co_await iter->second->load_one(mysql_pool, id);
        }
    }
    else
    {
        throw celeritas_error{ "load from db is error, db name = {}", db_name };
    }
}

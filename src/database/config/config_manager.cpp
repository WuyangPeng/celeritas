#include "config_manager.h"
#include "common/logging/logger.h"
#include "database/database_pool_manager.h"
#include "database/generated/mysql/player/user.h"

celeritas::config_manager& celeritas::config_manager::get_instance()
{
    static config_manager instance{};

    return instance;
}

void celeritas::config_manager::reload_from_db(io_context_type& io_context, const std::string& db_name, int64_t id)
{
    if (db_name.empty() && id == 0)
    {
        load_from_db(io_context);
    }

    boost::asio::co_spawn(io_context,
                          [db_name,id,this] {
                              return this->load_from_db(db_name, id);
                          }, boost::asio::detached);
}

void celeritas::config_manager::load_from_db(io_context_type& io_context)
{
    boost::asio::co_spawn(io_context,
                          [this] {
                              return this->load_from_db();
                          }, boost::asio::detached);
}

celeritas::config_manager::optional_time_refresh celeritas::config_manager::get_time_refresh(const int64_t id)
{
    std::shared_lock lock{ shared_mutex_ };

    if (const auto iter = time_refresh_.find(id);
        iter != time_refresh_.cend())
    {
        return iter->second;
    }

    return std::nullopt;
}

celeritas::config_manager::void_awaitable_type celeritas::config_manager::load_from_db()
{
    try
    {
        co_return co_await do_load_from_db();
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(database_channel, error) << "load config from db error: " << error.what();
    }
    catch (...)
    {
        LOG_CHANNEL(database_channel, fatal) << "load config from db unknown error.";
    }
}

celeritas::config_manager::void_awaitable_type celeritas::config_manager::do_load_from_db()
{
    const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_config_db_name.data());

    co_await do_load_time_refresh_db(mysql_pool);
}

celeritas::config_manager::void_awaitable_type celeritas::config_manager::do_load_time_refresh_db(const database_pool_shared_ptr& mysql_pool)
{
    const auto time_refresh_result = co_await mysql_pool->select_all(time_refresh::get_select(database_type::mysql), time_refresh::get_database_field_container());

    time_refresh_container_type container{};
    for (const auto& row : time_refresh_result)
    {
        const time_refresh time_refresh{ row };
        container.emplace(time_refresh.get_id(), time_refresh);
    }

    std::unique_lock lock{ shared_mutex_ };
    time_refresh_ = std::move(container);
}

celeritas::config_manager::void_awaitable_type celeritas::config_manager::load_from_db(const std::string& db_name, const int64_t id)
{
    try
    {
        co_return co_await do_load_from_db(db_name, id);
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(auth_channel, error) << "load config from db error: " << error.what();
    }
    catch (...)
    {
        LOG_CHANNEL(auth_channel, fatal) << "load config from db unknown error.";
    }
}

celeritas::config_manager::void_awaitable_type celeritas::config_manager::do_load_from_db(const std::string& db_name, const int64_t id)
{
    const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_config_db_name.data());

    if (db_name.empty())
    {
        co_await do_load_time_refresh_db(mysql_pool, id);
    }
    else if (db_name == time_refresh_db_name)
    {
        co_await do_load_time_refresh_db(mysql_pool, id);
    }
}

celeritas::config_manager::void_awaitable_type celeritas::config_manager::do_load_time_refresh_db(const database_pool_shared_ptr& mysql_pool, const int64_t id)
{
    if (id == 0)
    {
        co_return co_await do_load_time_refresh_db(mysql_pool);
    }

    if (const auto optional_time_refresh = co_await mysql_pool->select_one(time_refresh::get_select(database_type::mysql, id), time_refresh::get_database_field_container()))
    {
        const time_refresh time_refresh{ *optional_time_refresh };

        std::unique_lock lock{ shared_mutex_ };
        time_refresh_.emplace(time_refresh.get_id(), time_refresh);
    }
}
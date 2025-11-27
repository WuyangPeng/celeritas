#include "app_secret.h"
#include "app_status_type.h"
#include "common/celeritas_error.h"
#include "common/logger.h"
#include "database/database_pool_manager.h"

celeritas::app_secret& celeritas::app_secret::get_instance()
{
    static app_secret instance{};

    return instance;
}

std::string celeritas::app_secret::get_key(const int64_t app_id)
{
    std::shared_lock lock{ mutex_ };

    if (const auto iter = apps_.find(app_id);
        iter != apps_.cend())
    {
        if (iter->second.get_status() == static_cast<int>(app_status_type::close))
        {
            throw celeritas_error("app is close.");
        }
        return iter->second.get_app_secret();
    }

    throw celeritas_error("app_secret not registered");
}

void celeritas::app_secret::load_from_db(io_context_type& io_context)
{
    boost::asio::co_spawn(io_context,
                          [] {
                              return app_secret::get_instance().load_from_db();
                          }, boost::asio::detached);
}

celeritas::app_secret::void_awaitable_type celeritas::app_secret::load_from_db()
{
    try
    {
        co_return co_await do_load_from_db();
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(auth_channel, error) << "load app secret from db error: " << error.what();
    }
    catch (...)
    {
        LOG_CHANNEL(auth_channel, fatal) << "load app secret from db unknown error.";
    }
}

celeritas::app_secret::void_awaitable_type celeritas::app_secret::do_load_from_db()
{
    const auto mysql_pool = database_pool_manager::get_instance().get_pool(auth_db_name.data());
    const auto select = apps::get_select(database_type::mysql);
    const auto result = co_await mysql_pool->select_all(select, apps::get_database_field_container());

    apps_type apps_type{};
    for (const auto& row : result)
    {
        const apps app{ row };
        apps_type.emplace(app.get_app_id(), app);
    }

    std::unique_lock lock{ mutex_ };
    apps_ = apps_type;
}

celeritas::app_secret::void_awaitable_type celeritas::app_secret::load_from_db(const int64_t app_id)
{
    try
    {
        co_return co_await do_load_from_db(app_id);
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(auth_channel, error) << "load app secret from db error: " << error.what();
    }
    catch (...)
    {
        LOG_CHANNEL(auth_channel, fatal) << "load app secret from db unknown error.";
    }
}

celeritas::app_secret::void_awaitable_type celeritas::app_secret::do_load_from_db(const int64_t app_id)
{
    const auto mysql_pool = database_pool_manager::get_instance().get_pool(auth_db_name.data());
    const auto select = apps::get_select(database_type::mysql, app_id);

    if (const auto result = co_await mysql_pool->select_one(select, apps::get_database_field_container()))
    {
        const apps app{ *result };
        std::unique_lock lock{ mutex_ };
        apps_.emplace(app.get_app_id(), app);
    }
}

void celeritas::app_secret::reload_from_db(io_context_type& io_context, const int64_t app_id)
{
    if (app_id == 0)
    {
        load_from_db(io_context);
    }

    boost::asio::co_spawn(io_context,
                          [app_id] {
                              return app_secret::get_instance().load_from_db(app_id);
                          }, boost::asio::detached);
}
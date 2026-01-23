#include "app_secret.h"
#include "auth/core/app_status_type.h"
#include "common/core/celeritas_error.h"
#include "common/core/noexcept_safe_call_and_log.h"
#include "common/logging/logger.h"
#include "database/database_constant.h"
#include "database/pool/database_pool_manager.h"

celeritas::app_secret& celeritas::app_secret::get_instance()
{
    static app_secret instance{};

    return instance;
}

std::string celeritas::app_secret::get_key(const int64_t app_id)
{
    return get_apps(app_id).get_app_secret();
}

celeritas::apps celeritas::app_secret::get_apps(const int64_t app_id)
{
    std::shared_lock lock{ mutex_ };

    if (const auto iter = apps_.find(app_id);
        iter != apps_.cend())
    {
        if (iter->second.get_status() == static_cast<int>(app_status_type::close))
        {
            throw celeritas_error{ "app is close." };
        }
        return iter->second;
    }

    throw celeritas_error{ "app secret not registered" };
}

void celeritas::app_secret::reload_from_db(const any_io_executor& any_io_executor, const int64_t app_id)
{
    if (app_id == 0)
    {
        load_from_db(any_io_executor);
    }

    boost::asio::co_spawn(any_io_executor,
                          noexcept_safe_call_and_log_awaitable([app_id] {
                                                                   return get_instance().load_from_db(app_id);
                                                               },
                                                               auth_channel,
                                                               "load app secret from db error:"),
                          boost::asio::detached);
}

void celeritas::app_secret::load_from_db(const any_io_executor& any_io_executor)
{
    boost::asio::co_spawn(any_io_executor,
                          noexcept_safe_call_and_log_awaitable([] {
                                                                   return get_instance().load_from_db();
                                                               },
                                                               auth_channel,
                                                               "load app secret from db error:"),
                          boost::asio::detached);
}

celeritas::app_secret::void_awaitable_type celeritas::app_secret::load_from_db()
{
    const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_auth_db_name.data());

    const auto apps_result = co_await mysql_pool->select_all(apps::get_select(database_type::mysql), apps::get_database_field_container());

    apps_type apps_type{};
    for (const auto& row : apps_result)
    {
        const apps app{ row };
        apps_type.emplace(app.get_app_id(), app);
    }

    std::lock_guard lock{ mutex_ };
    apps_ = std::move(apps_type);
}

celeritas::app_secret::void_awaitable_type celeritas::app_secret::load_from_db(const int64_t app_id)
{
    const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_auth_db_name.data());

    if (const auto apps_result = co_await mysql_pool->select_one(apps::get_select(database_type::mysql, app_id), apps::get_database_field_container()))
    {
        const apps app{ *apps_result };

        std::lock_guard lock{ mutex_ };
        apps_.emplace(app.get_app_id(), app);
    }
}
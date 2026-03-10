#include "app_secret.h"
#include "auth/core/app_status_type.h"
#include "common/core/celeritas_error.h"
#include "common/core/enum_cast.h"
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
    return get_apps(app_id)->get_app_secret();
}

celeritas::app_secret::const_apps_shared_ptr celeritas::app_secret::get_apps(const int64_t app_id)
{
    std::shared_lock lock{ mutex_ };

    if (const auto iter = apps_.find(app_id);
        iter != apps_.cend())
    {
        if (iter->second->get_status() == enum_cast_underlying(app_status_type::close))
        {
            throw celeritas_error{ "app is close. app_id = {}.", app_id };
        }
        return iter->second;
    }

    throw celeritas_error{ "app secret not registered. app_id = {}.", app_id };
}

void celeritas::app_secret::reload_from_db(const any_io_executor& any_io_executor, const int64_t app_id)
{
    if (app_id == 0)
    {
        load_from_db(any_io_executor);
        return;
    }

    safe_co_spawn(any_io_executor,
                  [app_id] {
                      return get_instance().load_from_db(app_id);
                  },
                  auth_channel,
                  "load app secret from db error, app_id = {}.",
                  app_id);
}

void celeritas::app_secret::load_from_db(const any_io_executor& any_io_executor)
{
    safe_co_spawn(any_io_executor,
                  [] {
                      return get_instance().load_from_db();
                  },
                  auth_channel,
                  "load app secret from db error:");
}

celeritas::app_secret::void_awaitable_type celeritas::app_secret::load_from_db()
{
    const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_auth_db_name.data());

    const auto apps_result = co_await mysql_pool->select_all<apps>(database_type::mysql);

    auto container = get_app_providers_container(apps_result);

    std::lock_guard lock{ mutex_ };
    apps_ = std::move(container);
}

celeritas::app_secret::void_awaitable_type celeritas::app_secret::load_from_db(const int64_t app_id)
{
    const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_auth_db_name.data());

    if (const auto optional_provider = co_await mysql_pool->select_one<apps>(database_type::mysql, app_id))
    {
        add_app_provider(optional_provider);
    }
    else
    {
        LOG_CHANNEL(auth_channel, warning) << "app provider not found in database, app_id = " << app_id;
    }
}

void celeritas::app_secret::add_app_provider(const optional_database_entity_change& optional_provider)
{
    auto app = std::make_shared<apps>(*optional_provider);
    const auto app_id = app->get_app_id();

    LOG_CHANNEL(auth_channel, info) << "loaded app provider from db, app id = " << app_id;

    std::lock_guard lock{ mutex_ };
    apps_.insert_or_assign(app_id, std::move(app));
}

celeritas::app_secret::apps_container celeritas::app_secret::get_app_providers_container(const database_entity_change_container& apps_result)
{
    apps_container container{};
    container.reserve(apps_result.size());

    for (const auto& row : apps_result)
    {
        const auto app = std::make_shared<apps>(row);
        const auto app_id = app->get_app_id();
        container.emplace(app_id, app);

        LOG_CHANNEL(auth_channel, info) << "loaded app provider from db, app id = " << app_id;
    }

    return container;
}
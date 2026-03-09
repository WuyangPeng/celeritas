#include "app_sdk_providers.h"
#include "auth/sdk/sdk_process_type.h"
#include "common/core/celeritas_error.h"
#include "common/core/noexcept_safe_call_and_log.h"
#include "common/logging/logger.h"
#include "database/database_constant.h"
#include "database/pool/database_pool_manager.h"

celeritas::app_sdk_providers& celeritas::app_sdk_providers::get_instance()
{
    static app_sdk_providers instance{};

    return instance;
}

celeritas::app_sdk_providers::const_sdk_providers_shared_ptr celeritas::app_sdk_providers::get_sdk_providers(const sdk_providers_key& sdk_providers_key)
{
    std::shared_lock lock{ mutex_ };

    if (const auto iter = sdk_providers_.find(sdk_providers_key);
        iter != sdk_providers_.cend())
    {
        if (!iter->second->is_active())
        {
            throw celeritas_error{ "sdk providers is close. app_id = {}, sdk_process_type = {}", sdk_providers_key.get_app_id(), get_sdk_process_type_description(sdk_providers_key.get_sdk_process_type()) };
        }
        return iter->second;
    }

    throw celeritas_error{ "sdk providers not registered. app_id = {},sdk_process_type = {}", sdk_providers_key.get_app_id(), get_sdk_process_type_description(sdk_providers_key.get_sdk_process_type()) };
}

void celeritas::app_sdk_providers::reload_from_db(const any_io_executor& any_io_executor, int64_t sdk_id)
{
    if (sdk_id == 0)
    {
        load_from_db(any_io_executor);
    }

    safe_co_spawn(any_io_executor,
                  [sdk_id] {
                      return get_instance().load_from_db(sdk_id);
                  },
                  auth_channel,
                  "load sdk providers from db error,sdk_id = {}.",
                  sdk_id);
}

void celeritas::app_sdk_providers::load_from_db(const any_io_executor& any_io_executor)
{
    safe_co_spawn(any_io_executor,
                  [] {
                      return get_instance().load_from_db();
                  },
                  auth_channel,
                  "load sdk providers from db error:");
}

celeritas::app_sdk_providers::void_awaitable_type celeritas::app_sdk_providers::load_from_db()
{
    const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_auth_db_name.data());

    const auto apps_result = co_await mysql_pool->select_all(sdk_providers::get_select(database_type::mysql), sdk_providers::get_database_field_container());

    auto container = get_sdk_providers_container(apps_result);

    std::lock_guard lock{ mutex_ };
    sdk_providers_ = std::move(container);
}

celeritas::app_sdk_providers::void_awaitable_type celeritas::app_sdk_providers::load_from_db(const int64_t sdk_id)
{
    const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_auth_db_name.data());

    if (const auto optional_sdk_providers = co_await mysql_pool->select_one(sdk_providers::get_select(database_type::mysql, sdk_id), sdk_providers::get_database_field_container()))
    {
        add_sdk_provider(optional_sdk_providers);
    }
    else
    {
        LOG_CHANNEL(auth_channel, warning) << "sdk provider not found in database, sdk_id = " << sdk_id;
    }
}

void celeritas::app_sdk_providers::add_sdk_provider(const optional_database_entity_change& optional_provider)
{
    auto provider = std::make_shared<sdk_providers>(*optional_provider);

    const auto sdk_id = provider->get_sdk_id();

    LOG_CHANNEL(auth_channel, info) << "loaded sdk provider from db, sdk id = " << sdk_id;

    std::lock_guard lock{ mutex_ };
    erase_if(sdk_providers_, [sdk_id](const auto& element) {
        return element.second->get_sdk_id() == sdk_id;
    });

    sdk_providers_.emplace(sdk_providers_key{ provider->get_app_id(), static_cast<sdk_process_type>(provider->get_process_type()) }, provider);
}

celeritas::app_sdk_providers::sdk_providers_container celeritas::app_sdk_providers::get_sdk_providers_container(const database_entity_change_container& apps_result)
{
    sdk_providers_container sdk_providers_type{};
    for (const auto& row : apps_result)
    {
        const auto provider = std::make_shared<sdk_providers>(row);
        sdk_providers_type.emplace(sdk_providers_key{ provider->get_app_id(), static_cast<sdk_process_type>(provider->get_process_type()) }, provider);

        LOG_CHANNEL(auth_channel, info) << "loaded sdk provider from db, sdk id = " << provider->get_sdk_id();
    }

    return sdk_providers_type;
}
#include "app_email_providers.h"
#include "common/core/celeritas_error.h"
#include "common/core/noexcept_safe_call_and_log.h"
#include "common/logging/logger.h"
#include "database/database_constant.h"
#include "database/pool/database_pool_manager.h"

celeritas::app_email_providers& celeritas::app_email_providers::get_instance()
{
    static app_email_providers instance{};

    return instance;
}

celeritas::app_email_providers::const_email_providers_shared_ptr celeritas::app_email_providers::get_email_providers(const int64_t provider_id)
{
    std::shared_lock lock{ mutex_ };

    if (const auto iter = email_providers_.find(provider_id);
        iter != email_providers_.cend())
    {
        if (!iter->second->is_active())
        {
            throw celeritas_error{ "email providers is close." };
        }
        return iter->second;
    }

    throw celeritas_error{ "email providers not registered" };
}

void celeritas::app_email_providers::reload_from_db(const any_io_executor& any_io_executor, const int64_t provider_id)
{
    if (provider_id == 0)
    {
        load_from_db(any_io_executor);
        return;
    }

    safe_co_spawn(any_io_executor,
                  [provider_id] {
                      return get_instance().load_from_db(provider_id);
                  },
                  auth_channel,
                  "load email providers from db error, provider_id: {}",
                  provider_id);
}

void celeritas::app_email_providers::load_from_db(const any_io_executor& any_io_executor)
{
    safe_co_spawn(any_io_executor,
                  [] {
                      return get_instance().load_from_db();
                  },
                  auth_channel,
                  "load email providers from db error");
}

celeritas::app_email_providers::email_providers_container celeritas::app_email_providers::get_email_providers_container(const database_entity_change_container& apps_result)
{
    email_providers_container container{};
    container.reserve(apps_result.size());

    for (const auto& row : apps_result)
    {
        const auto provider = std::make_shared<email_providers>(row);
        const auto provider_id = provider->get_provider_id();
        container.emplace(provider_id, provider);

        LOG_CHANNEL(auth_channel, info) << "loaded email provider from db, provider_id: " << provider_id;
    }

    return container;
}

celeritas::app_email_providers::void_awaitable_type celeritas::app_email_providers::load_from_db()
{
    const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_auth_db_name.data());

    const auto apps_result = co_await mysql_pool->select_all<email_providers>(database_type::mysql);

    auto container = get_email_providers_container(apps_result);

    std::lock_guard lock{ mutex_ };
    email_providers_ = std::move(container);
}

void celeritas::app_email_providers::add_email_provider(const optional_database_entity_change& optional_provider)
{
    auto provider = std::make_shared<email_providers>(*optional_provider);
    const auto provider_id = provider->get_provider_id();

    LOG_CHANNEL(auth_channel, info) << "loaded email provider from db, provider_id: " << provider_id;

    std::lock_guard lock{ mutex_ };
    email_providers_.insert_or_assign(provider_id, std::move(provider));
}

celeritas::app_email_providers::void_awaitable_type celeritas::app_email_providers::load_from_db(const int64_t provider_id)
{
    const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_auth_db_name.data());

    if (const auto optional_provider = co_await mysql_pool->select_one<email_providers>(database_type::mysql, provider_id))
    {
        add_email_provider(optional_provider);
    }
    else
    {
        LOG_CHANNEL(auth_channel, warning) << "email provider not found in database, provider_id: " << provider_id;
    }
}
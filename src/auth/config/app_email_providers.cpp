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

celeritas::email_providers celeritas::app_email_providers::get_email_providers(const int64_t provider_id)
{
    std::shared_lock lock{ mutex_ };

    if (const auto iter = email_providers_.find(provider_id);
        iter != email_providers_.cend())
    {
        if (!iter->second.is_active())
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
                  "load email providers from db error");
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

celeritas::app_email_providers::void_awaitable_type celeritas::app_email_providers::load_from_db()
{
    const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_auth_db_name.data());

    const auto apps_result = co_await mysql_pool->select_all<email_providers>(database_type::mysql);

    email_providers_container container{};
    for (const auto& row : apps_result)
    {
        const email_providers email_providers{ row };
        container.emplace(email_providers.get_provider_id(), email_providers);
    }

    std::lock_guard lock{ mutex_ };
    email_providers_ = std::move(container);
}

celeritas::app_email_providers::void_awaitable_type celeritas::app_email_providers::load_from_db(const int64_t provider_id)
{
    const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_auth_db_name.data());

    if (const auto optional_sms_providers = co_await mysql_pool->select_one<email_providers>(database_type::mysql, provider_id))
    {
        const email_providers email_providers{ *optional_sms_providers };

        std::lock_guard lock{ mutex_ };
        email_providers_.emplace(email_providers.get_provider_id(), email_providers);
    }
}
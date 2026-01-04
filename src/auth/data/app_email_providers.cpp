#include "app_email_providers.h"
#include "common/core/celeritas_error.h"
#include "common/logging/logger.h"
#include "database/database_pool_manager.h"

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
        if (iter->second.is_active())
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
    }

    boost::asio::co_spawn(any_io_executor,
                          [provider_id,this] {
                              return this->load_from_db(provider_id);
                          }, boost::asio::detached);
}

void celeritas::app_email_providers::load_from_db(const any_io_executor& any_io_executor)
{
    boost::asio::co_spawn(any_io_executor,
                          [this] {
                              return this->load_from_db();
                          }, boost::asio::detached);
}

celeritas::app_email_providers::void_awaitable_type celeritas::app_email_providers::load_from_db()
{
    try
    {
        co_return co_await do_load_from_db();
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(auth_channel, error) << "load email providers from db error: " << error.what();
    }
    catch (...)
    {
        LOG_CHANNEL(auth_channel, fatal) << "load email providers from db unknown error.";
    }
}

celeritas::app_email_providers::void_awaitable_type celeritas::app_email_providers::do_load_from_db()
{
    const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_auth_db_name.data());

    const auto apps_result = co_await mysql_pool->select_all(email_providers::get_select(database_type::mysql), email_providers::get_database_field_container());

    email_providers_type email_providers_type{};
    for (const auto& row : apps_result)
    {
        const email_providers email_providers{ row };
        email_providers_type.emplace(email_providers.get_provider_id(), email_providers);
    }

    std::unique_lock lock{ mutex_ };
    email_providers_ = std::move(email_providers_type);
}

celeritas::app_email_providers::void_awaitable_type celeritas::app_email_providers::load_from_db(const int64_t provider_id)
{
    try
    {
        co_return co_await do_load_from_db(provider_id);
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(auth_channel, error) << "load sms providers from db error: " << error.what();
    }
    catch (...)
    {
        LOG_CHANNEL(auth_channel, fatal) << "load sms providers from db unknown error.";
    }
}

celeritas::app_email_providers::void_awaitable_type celeritas::app_email_providers::do_load_from_db(const int64_t provider_id)
{
    const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_auth_db_name.data());

    if (const auto optional_sms_providers = co_await mysql_pool->select_one(email_providers::get_select(database_type::mysql, provider_id), email_providers::get_database_field_container()))
    {
        const email_providers email_providers{ *optional_sms_providers };

        std::unique_lock lock{ mutex_ };
        email_providers_.emplace(email_providers.get_provider_id(), email_providers);
    }
}
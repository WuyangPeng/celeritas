#include "app_sms_providers.h"
#include "common/core_utilities/celeritas_error.h"
#include "common/logger.h"
#include "database/database_pool_manager.h"

celeritas::app_sms_providers& celeritas::app_sms_providers::get_instance()
{
    static app_sms_providers instance{};

    return instance;
}

celeritas::sms_providers celeritas::app_sms_providers::get_sms_providers(const int64_t provider_id)
{
    std::shared_lock lock{ mutex_ };

    if (const auto iter = sms_providers_.find(provider_id);
        iter != sms_providers_.cend())
    {
        if (iter->second.is_active())
        {
            throw celeritas_error{ "sms providers is close." };
        }
        return iter->second;
    }

    throw celeritas_error{ "sms providers not registered" };
}

void celeritas::app_sms_providers::reload_from_db(io_context_type& io_context, int64_t provider_id)
{
    if (provider_id == 0)
    {
        load_from_db(io_context);
    }

    boost::asio::co_spawn(io_context,
                          [provider_id,this] {
                              return this->load_from_db(provider_id);
                          }, boost::asio::detached);
}

void celeritas::app_sms_providers::load_from_db(io_context_type& io_context)
{
    boost::asio::co_spawn(io_context,
                          [this] {
                              return this->load_from_db();
                          }, boost::asio::detached);
}

celeritas::app_sms_providers::void_awaitable_type celeritas::app_sms_providers::load_from_db()
{
    try
    {
        co_return co_await do_load_from_db();
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

celeritas::app_sms_providers::void_awaitable_type celeritas::app_sms_providers::do_load_from_db()
{
    const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_auth_db_name.data());

    const auto apps_result = co_await mysql_pool->select_all(sms_providers::get_select(database_type::mysql),
                                                             sms_providers::get_database_field_container());

    sms_providers_type sms_providers_type{};
    for (const auto& row : apps_result)
    {
        const sms_providers sms_providers{ row };
        sms_providers_type.emplace(sms_providers.get_provider_id(), sms_providers);
    }

    std::unique_lock lock{ mutex_ };
    sms_providers_ = std::move(sms_providers_type);
}

celeritas::app_sms_providers::void_awaitable_type celeritas::app_sms_providers::load_from_db(int64_t provider_id)
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

celeritas::app_sms_providers::void_awaitable_type celeritas::app_sms_providers::do_load_from_db(int64_t provider_id)
{
    const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_auth_db_name.data());

    if (const auto optional_sms_providers = co_await mysql_pool->select_one(
        sms_providers::get_select(database_type::mysql, provider_id), sms_providers::get_database_field_container()))
    {
        const sms_providers sms_providers{ *optional_sms_providers };

        std::unique_lock lock{ mutex_ };
        sms_providers_.emplace(sms_providers.get_provider_id(), sms_providers);
    }
}
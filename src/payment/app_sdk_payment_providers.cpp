#include "app_sdk_payment_providers.h"
#include "common/celeritas_error.h"
#include "common/logger.h"
#include "database/database_pool_manager.h"

celeritas::app_sdk_payment_providers& celeritas::app_sdk_payment_providers::get_instance()
{
    static app_sdk_payment_providers instance{};

    return instance;
}

celeritas::sdk_payment_providers celeritas::app_sdk_payment_providers::get_sdk_providers(const sdk_payment_providers_key& sdk_payment_providers_key)
{
    std::shared_lock lock{ mutex_ };

    if (const auto iter = sdk_payment_providers_.find(sdk_payment_providers_key);
        iter != sdk_payment_providers_.cend())
    {
        if (iter->second.is_active())
        {
            throw celeritas_error{ "sdk providers is close." };
        }
        return iter->second;
    }

    throw celeritas_error{ "sdk providers not registered" };
}

celeritas::sdk_payment_providers celeritas::app_sdk_payment_providers::get_sdk_providers(int64_t sdk_id)
{
    std::shared_lock lock{ mutex_ };

    const auto iter = std::ranges::find_if(sdk_payment_providers_, [sdk_id](const auto& element) {
        return element.second.get_sdk_id() == sdk_id;
    });

    if (iter != sdk_payment_providers_.cend())
    {
        return iter->second;
    }

    throw celeritas_error{ "sdk providers not registered,sdk id = {}", sdk_id };
}

void celeritas::app_sdk_payment_providers::reload_from_db(io_context_type& io_context, int64_t sdk_id)
{
    if (sdk_id == 0)
    {
        load_from_db(io_context);
    }

    boost::asio::co_spawn(io_context,
                          [sdk_id,this] {
                              return this->load_from_db(sdk_id);
                          }, boost::asio::detached);
}

void celeritas::app_sdk_payment_providers::load_from_db(io_context_type& io_context)
{
    boost::asio::co_spawn(io_context,
                          [this] {
                              return this->load_from_db();
                          }, boost::asio::detached);
}

celeritas::app_sdk_payment_providers::sdk_payment_providers_type celeritas::app_sdk_payment_providers::get_sdk_payment_providers()
{
    std::shared_lock lock{ mutex_ };

    return sdk_payment_providers_;
}

celeritas::app_sdk_payment_providers::void_awaitable_type celeritas::app_sdk_payment_providers::load_from_db()
{
    try
    {
        co_return co_await do_load_from_db();
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(payment_channel, error) << "load sdk providers from db error: " << error.what();
    }
    catch (...)
    {
        LOG_CHANNEL(payment_channel, fatal) << "load sdk providers from db unknown error.";
    }
}

celeritas::app_sdk_payment_providers::void_awaitable_type celeritas::app_sdk_payment_providers::do_load_from_db()
{
    const auto mysql_pool = database_pool_manager::get_instance().get_pool(payment_db_name.data());

    const auto apps_result = co_await mysql_pool->select_all(sdk_payment_providers::get_select(database_type::mysql), sdk_payment_providers::get_database_field_container());

    sdk_payment_providers_type sdk_payment_providers_type{};
    for (const auto& row : apps_result)
    {
        const sdk_payment_providers sdk_payment_providers{ row };
        sdk_payment_providers_type.emplace(sdk_payment_providers_key{ sdk_payment_providers.get_app_id(), static_cast<payment_platform_type>(sdk_payment_providers.get_platform()) }, sdk_payment_providers);
    }

    std::unique_lock lock{ mutex_ };
    sdk_payment_providers_ = std::move(sdk_payment_providers_type);
}

celeritas::app_sdk_payment_providers::void_awaitable_type celeritas::app_sdk_payment_providers::load_from_db(int64_t sdk_id)
{
    try
    {
        co_return co_await do_load_from_db(sdk_id);
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(payment_channel, error) << "load sdk providers from db error: " << error.what();
    }
    catch (...)
    {
        LOG_CHANNEL(payment_channel, fatal) << "load sdk providers from db unknown error.";
    }
}

celeritas::app_sdk_payment_providers::void_awaitable_type celeritas::app_sdk_payment_providers::do_load_from_db(int64_t sdk_id)
{
    const auto mysql_pool = database_pool_manager::get_instance().get_pool(payment_db_name.data());

    if (const auto optional_sdk_payment_providers = co_await mysql_pool->select_one(sdk_payment_providers::get_select(database_type::mysql, sdk_id), sdk_payment_providers::get_database_field_container()))
    {
        const sdk_payment_providers sdk_payment_providers{ *optional_sdk_payment_providers };

        std::unique_lock lock{ mutex_ };
        erase_if(sdk_payment_providers_, [sdk_id](const auto& element) {
            return element.second.get_sdk_id() == sdk_id;
        });

        sdk_payment_providers_.emplace(sdk_payment_providers_key{ sdk_payment_providers.get_app_id(), static_cast<payment_platform_type>(sdk_payment_providers.get_platform()) }, sdk_payment_providers);
    }
}
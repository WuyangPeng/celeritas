#include "app_sdk_providers.h"
#include "common/celeritas_error.h"
#include "common/logger.h"
#include "database/database_pool_manager.h"

celeritas::app_sdk_providers& celeritas::app_sdk_providers::get_instance()
{
    static app_sdk_providers instance{};

    return instance;
}

celeritas::sdk_providers celeritas::app_sdk_providers::get_sdk_providers(const sdk_providers_key& sdk_providers_key)
{
    std::shared_lock lock{ mutex_ };

    if (const auto iter = sdk_providers_.find(sdk_providers_key);
        iter != sdk_providers_.cend())
    {
        if (iter->second.is_active())
        {
            throw celeritas_error{ "sdk providers is close." };
        }
        return iter->second;
    }

    throw celeritas_error{ "sdk providers not registered" };
}

void celeritas::app_sdk_providers::reload_from_db(io_context_type& io_context, int64_t sdk_id)
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

void celeritas::app_sdk_providers::load_from_db(io_context_type& io_context)
{
    boost::asio::co_spawn(io_context,
                          [this] {
                              return this->load_from_db();
                          }, boost::asio::detached);
}

celeritas::app_sdk_providers::void_awaitable_type celeritas::app_sdk_providers::load_from_db()
{
    try
    {
        co_return co_await do_load_from_db();
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(auth_channel, error) << "load sdk providers from db error: " << error.what();
    }
    catch (...)
    {
        LOG_CHANNEL(auth_channel, fatal) << "load sdk providers from db unknown error.";
    }
}

celeritas::app_sdk_providers::void_awaitable_type celeritas::app_sdk_providers::do_load_from_db()
{
    const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_auth_db_name.data());

    const auto apps_result = co_await mysql_pool->select_all(sdk_providers::get_select(database_type::mysql), sdk_providers::get_database_field_container());

    sdk_providers_type sdk_providers_type{};
    for (const auto& row : apps_result)
    {
        const sdk_providers sdk_providers{ row };
        sdk_providers_type.emplace(sdk_providers_key{ sdk_providers.get_app_id(), static_cast<sdk_process_type>(sdk_providers.get_process_type()) }, sdk_providers);
    }

    std::unique_lock lock{ mutex_ };
    sdk_providers_ = std::move(sdk_providers_type);
}

celeritas::app_sdk_providers::void_awaitable_type celeritas::app_sdk_providers::load_from_db(const int64_t sdk_id)
{
    try
    {
        co_return co_await do_load_from_db(sdk_id);
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(auth_channel, error) << "load sdk providers from db error: " << error.what();
    }
    catch (...)
    {
        LOG_CHANNEL(auth_channel, fatal) << "load sdk providers from db unknown error.";
    }
}

celeritas::app_sdk_providers::void_awaitable_type celeritas::app_sdk_providers::do_load_from_db(int64_t sdk_id)
{
    const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_auth_db_name.data());

    if (const auto optional_sdk_providers = co_await mysql_pool->select_one(sdk_providers::get_select(database_type::mysql, sdk_id), sdk_providers::get_database_field_container()))
    {
        const sdk_providers sdk_providers{ *optional_sdk_providers };

        std::unique_lock lock{ mutex_ };
        erase_if(sdk_providers_, [sdk_id](const auto& element) {
            return element.second.get_sdk_id() == sdk_id;
        });

        sdk_providers_.emplace(sdk_providers_key{ sdk_providers.get_app_id(), static_cast<sdk_process_type>(sdk_providers.get_process_type()) }, sdk_providers);
    }
}
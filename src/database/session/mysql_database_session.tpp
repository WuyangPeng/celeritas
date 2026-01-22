#pragma once

#include "mysql_database_session.h"
#include "common/logging/logger.h"

template <typename Func>
auto celeritas::mysql_database_session::execute_with_retry(Func&& func) -> decltype(func())
{
    std::optional<error_code_type> retry_error{};

    try
    {
        co_return co_await func();
    }
    catch (const boost::system::system_error& error)
    {
        LOG_CHANNEL(database_channel, error) << "async query exception" << error.what();

        retry_error = error.code();
    }
    catch (...)
    {
        LOG_CHANNEL(database_channel, fatal) << "async_query unknown exception";
        throw;
    }

    if (retry_error.has_value())
    {
        co_await async_reconnect_on_disconnection(retry_error.value());

        co_return co_await func();
    }

    throw celeritas_error{ "mysql async query unknown exception" };
}
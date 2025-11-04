#include "redis_database_session.h"
#include "common/celeritas_error.h"
#include "common/common_fwd.h"
#include "common/logger.h"
#include "detail/redis_reply.h"

celeritas::redis_database_session::redis_database_session(const std::string_view& host,
                                                          int port,
                                                          const std::string_view& user,
                                                          const std::string_view& password,
                                                          const std::string_view& uri,
                                                          const std::string_view& db_name,
                                                          io_context_type& io_context)
    : redis_context_{}, io_context_{ io_context }, host_{ host }, password_{ password }, user_{ user }, port_{ port }, db_name_{ db_name }
{
}

celeritas::redis_database_session::void_awaitable_type celeritas::redis_database_session::async_connect()
{
    co_await boost::asio::post(io_context_, boost::asio::use_awaitable);

    redis_context_ = std::make_unique<redis_context>(host_, port_);

    const auto command = user_.empty() ? "AUTH " + password_ : "AUTH " + user_ + " " + password_;

    redis_reply redis_reply{ *redis_context_.get(), command };

    LOG_CHANNEL(database_channel, info) << "Authentication successful (AUTH: OK).";
}

celeritas::database_session::bool_awaitable_type celeritas::redis_database_session::is_health()
{
    co_await boost::asio::post(io_context_, boost::asio::use_awaitable);

    if (!redis_context_)
    {
        co_return false;
    }

    try
    {
        redis_reply redis_reply{ *redis_context_.get(), "PING" };

        co_return true;
    }
    catch (const celeritas_error& error)
    {
        LOG_CHANNEL(database_channel, warning) << "Redis health check failed with celeritas error: " << error.what();
        co_return false;
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(database_channel, error) << "Redis health check failed with unexpected exception: " << error.what();
        co_return false;
    }
    catch (...)
    {
        LOG_CHANNEL(database_channel, fatal) << "Redis health check failed with unknown exception";
        co_return false;
    }
}


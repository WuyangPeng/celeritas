#include "redis_database_session.h"
#include "common/celeritas_error.h"
#include "common/common_fwd.h"
#include "common/logger.h"
#include "config/database_type.h"
#include "detail/redis_reply.h"

celeritas::redis_database_session::redis_database_session(const std::string_view& host,
                                                          const int port,
                                                          const std::string_view& user,
                                                          const std::string_view& password,
                                                          const std::string_view& uri,
                                                          const std::string_view& db_name,
                                                          int expire_seconds,
                                                          io_context_type& io_context)
    : redis_context_{}, io_context_{ io_context }, host_{ host }, port_{ port }, user_{ user }, password_{ password }, db_name_{ db_name }, expire_seconds_{ expire_seconds }
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
    if (!redis_context_)
    {
        co_return false;
    }

    co_await boost::asio::post(io_context_, boost::asio::use_awaitable);

    try
    {
        redis_reply redis_reply{ *redis_context_.get(), "PING" };

        co_return true;
    }
    catch (const celeritas_error& error)
    {
        LOG_CHANNEL(database_channel, warning) << "Redis health check failed with celeritas error: " << error.what();
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(database_channel, error) << "Redis health check failed with exception: " << error.what();
    }
    catch (...)
    {
        LOG_CHANNEL(database_channel, fatal) << "Redis health check failed with unknown exception";
    }

    co_return false;
}

celeritas::redis_database_session::void_awaitable_type celeritas::redis_database_session::async_set(const std::string& key, const std::string& value, const int expire_seconds)
{
    const auto prefixed_key = get_prefixed_key(key);

    const auto set_command = std::string("SET ") + prefixed_key + " \"" + value + "\"" + get_expire_seconds_command(expire_seconds);

    co_await async_execute_command_return_int(set_command);

    co_return;
}

celeritas::redis_database_session::int_awaitable_type celeritas::redis_database_session::async_delete(const std::string& key)
{
    const auto prefixed_key = get_prefixed_key(key);

    const auto del_command = std::string("DEL ") + prefixed_key;

    co_return co_await async_execute_command_return_int(del_command);
}

celeritas::redis_database_session::int_awaitable_type celeritas::redis_database_session::async_delete_many(const container& keys)
{
    std::string command{};
    for (const auto& key : keys)
    {
        command += " " + get_prefixed_key(key);
    }

    const auto del_command = std::string("DEL") + command;

    co_return co_await async_execute_command_return_int(del_command);
}

celeritas::database_session::bool_awaitable_type celeritas::redis_database_session::set_expire_seconds(const std::string& key, const int expire_seconds)
{
    const auto prefixed_key = get_prefixed_key(key);

    const auto expire_command = std::string("EXPIRE ") + prefixed_key + " " + std::to_string(expire_seconds);

    const auto result = co_await async_execute_command_return_int(expire_command);

    co_return result > 0;
}

celeritas::redis_database_session::int_awaitable_type celeritas::redis_database_session::get_expire_seconds(const std::string& key)
{
    const auto prefixed_key = get_prefixed_key(key);

    const auto expire_command = std::string("TTL ") + prefixed_key;

    co_return co_await async_execute_command_return_int(expire_command);
}

celeritas::database_session::bool_awaitable_type celeritas::redis_database_session::is_exists(const std::string& key)
{
    const auto prefixed_key = get_prefixed_key(key);

    const auto exists_command = std::string("EXISTS ") + prefixed_key;

    const auto result = co_await async_execute_command_return_int(exists_command);

    co_return result > 0;
}

celeritas::redis_database_session::int_awaitable_type celeritas::redis_database_session::is_exists_many(const container& keys)
{
    std::string command{};
    for (const auto& key : keys)
    {
        command += " " + get_prefixed_key(key);
    }

    const auto exists_command = std::string("EXISTS") + command;

    co_return co_await async_execute_command_return_int(exists_command);
}

celeritas::redis_database_session::bool_awaitable_type celeritas::redis_database_session::rename(const std::string& old_key, const std::string& new_key)
{
    const auto old_prefixed_key = get_prefixed_key(old_key);
    const auto new_prefixed_key = get_prefixed_key(new_key);

    const auto rename_command = std::string("RENAME ") + old_prefixed_key + " " + new_prefixed_key;

    if (const auto result = co_await async_execute_command_return_string(rename_command);
        result == "OK")
    {
        co_return true;
    }

    co_return false;
}

celeritas::redis_database_session::string_awaitable_type celeritas::redis_database_session::get_type(const std::string& key)
{
    const auto prefixed_key = get_prefixed_key(key);

    const auto type_command = std::string("TYPE ") + prefixed_key;

    co_return co_await async_execute_command_return_string(type_command);
}

std::string celeritas::redis_database_session::get_prefixed_key(const std::string& key) const
{
    if (db_name_.empty())
    {
        return key;
    }

    return db_name_ + ":" + key;
}

celeritas::redis_database_session::int_awaitable_type celeritas::redis_database_session::async_execute_command_return_int(const std::string& command) const
{
    if (!redis_context_)
    {
        throw celeritas_error("redis context is not connected or initialized.");
    }

    co_await boost::asio::post(io_context_, boost::asio::use_awaitable);

    const redis_reply redis_reply{ *redis_context_.get(), command };

    co_return redis_reply.to_integer();
}

celeritas::redis_database_session::void_awaitable_type celeritas::redis_database_session::async_execute_command_return_void(const std::string& command) const
{
    if (!redis_context_)
    {
        throw celeritas_error("redis context is not connected or initialized.");
    }

    co_await boost::asio::post(io_context_, boost::asio::use_awaitable);

    const redis_reply redis_reply{ *redis_context_.get(), command };

    co_return;
}

celeritas::redis_database_session::string_awaitable_type celeritas::redis_database_session::async_execute_command_return_string(const std::string& command) const
{
    if (!redis_context_)
    {
        throw celeritas_error("redis context is not connected or initialized.");
    }

    co_await boost::asio::post(io_context_, boost::asio::use_awaitable);

    const redis_reply redis_reply{ *redis_context_.get(), command };

    if (auto result = redis_reply.to_optional_string())
    {
        co_return *result;
    }

    co_return "";
}

std::string celeritas::redis_database_session::get_expire_seconds_command(int expire_seconds) const
{
    if (expire_seconds == 0)
    {
        expire_seconds = expire_seconds_;
    }

    if (expire_seconds > 0)
    {
        return " EX " + std::to_string(expire_seconds);
    }

    return "";
}




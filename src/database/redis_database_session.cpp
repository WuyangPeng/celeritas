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
    : redis_context_{},
      io_context_{ io_context },
      host_{ host },
      port_{ port },
      user_{ user },
      password_{ password },
      db_name_{ db_name },
      expire_seconds_{ expire_seconds },
      redis_key_commands_{ *this },
      redis_string_commands_{ *this },
      redis_hash_commands_{ *this },
      redis_list_commands_{ *this },
      redis_set_commands_{ *this },
      redis_sorted_set_commands_{ *this }
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

    try
    {
        do_is_health();

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

celeritas::redis_key_commands& celeritas::redis_database_session::get_redis_key_commands()
{
    return redis_key_commands_;
}

celeritas::redis_string_commands& celeritas::redis_database_session::get_redis_string_commands()
{
    return redis_string_commands_;
}

celeritas::redis_hash_commands& celeritas::redis_database_session::get_redis_hash_commands()
{
    return redis_hash_commands_;
}

celeritas::redis_list_commands& celeritas::redis_database_session::get_redis_list_commands()
{
    return redis_list_commands_;
}

celeritas::redis_set_commands& celeritas::redis_database_session::get_redis_set_commands()
{
    return redis_set_commands_;
}

celeritas::redis_sorted_set_commands& celeritas::redis_database_session::get_redis_sorted_set_commands()
{
    return redis_sorted_set_commands_;
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
    check_initialized();

    co_await boost::asio::post(io_context_, boost::asio::use_awaitable);

    const redis_reply redis_reply{ *redis_context_.get(), command };

    co_return redis_reply.to_integer();
}

celeritas::redis_database_session::void_awaitable_type celeritas::redis_database_session::async_execute_command_return_void(const std::string& command) const
{
    check_initialized();

    co_await boost::asio::post(io_context_, boost::asio::use_awaitable);

    const redis_reply redis_reply{ *redis_context_.get(), command };

    co_return;
}

void celeritas::redis_database_session::check_initialized() const
{
    if (!redis_context_)
    {
        throw celeritas_error("redis context is not connected or initialized.");
    }
}

void celeritas::redis_database_session::do_is_health()
{
    check_initialized();

    redis_reply redis_reply{ *redis_context_.get(), "PING" };
}

celeritas::redis_database_session::optional_string_awaitable_type celeritas::redis_database_session::async_execute_command_return_optional_string(const std::string& command) const
{
    check_initialized();

    co_await boost::asio::post(io_context_, boost::asio::use_awaitable);

    const redis_reply redis_reply{ *redis_context_.get(), command };

    co_return redis_reply.to_optional_string();
}

celeritas::redis_database_session::array_type_awaitable_type celeritas::redis_database_session::async_execute_command_return_array_type(const std::string& command) const
{
    check_initialized();

    co_await boost::asio::post(io_context_, boost::asio::use_awaitable);

    const redis_reply redis_reply{ *redis_context_.get(), command };

    co_return redis_reply.to_array();
}

celeritas::redis_database_session::map_type_awaitable_type celeritas::redis_database_session::async_execute_command_return_map_type(const std::string& command) const
{
    check_initialized();

    co_await boost::asio::post(io_context_, boost::asio::use_awaitable);

    const redis_reply redis_reply{ *redis_context_.get(), command };

    co_return redis_reply.to_map();
}

celeritas::redis_database_session::optional_double_awaitable_type celeritas::redis_database_session::async_execute_command_return_optional_double(const std::string& command) const
{
    check_initialized();

    co_await boost::asio::post(io_context_, boost::asio::use_awaitable);

    const redis_reply redis_reply{ *redis_context_.get(), command };

    co_return redis_reply.to_optional_double();
}

celeritas::redis_database_session::int_awaitable_type celeritas::redis_database_session::async_execute_command_return_optional_int(const std::string& command) const
{
    check_initialized();

    co_await boost::asio::post(io_context_, boost::asio::use_awaitable);

    const redis_reply redis_reply{ *redis_context_.get(), command };

    co_return redis_reply.to_optional_int();
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




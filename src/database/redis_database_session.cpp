#include "basis_database_manager.h"
#include "database_change_type.h"
#include "database_data_type.h"
#include "database_field.h"
#include "redis_database_session.h"
#include "common/celeritas_error.h"
#include "common/common_fwd.h"
#include "common/logger.h"
#include "config/database_type.h"
#include "detail/redis_reply.h"
#include "basis_database.tpp"

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/lexical_cast.hpp>

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

std::string celeritas::redis_database_session::generate_key(const basis_database_manager_shared_ptr& database)
{
    std::string result{};
    result += database->get_database_name();
    result += ":";

    auto index = 1;
    for (const auto key = database->get_key();
         const auto& value : key)
    {
        result += value.get_quotation_mark_string();

        if (index != key.get_size())
        {
            result += "_";
        }

        ++index;
    }

    return result;
}

celeritas::basis_database celeritas::redis_database_session::get_basis_database(const database_field& field_name, const std::string& value)
{
    switch (field_name.get_data_type())
    {
        case database_data_type::string_type:
            return basis_database{ field_name.get_field_name(), value };

        case database_data_type::int32_type:
        case database_data_type::int32_count_type:
            return basis_database{ field_name.get_field_name(), boost::lexical_cast<int32_t>(value) };

        case database_data_type::int64_type:
        case database_data_type::int64_count_type:
            return basis_database{ field_name.get_field_name(), boost::lexical_cast<int64_t>(value) };

        case database_data_type::double_type:
            return basis_database{ field_name.get_field_name(), boost::lexical_cast<double>(value) };

        case database_data_type::bool_type:
            return basis_database{ field_name.get_field_name(), value == "true" };

        case database_data_type::string_array_type:
        {
            const std::string column{ value };
            basis_database::string_array element{};
            split(element, column, boost::is_any_of("|"), boost::token_compress_off);

            return basis_database{ field_name.get_field_name(), element };
        }

        case database_data_type::int32_array_type:
        {
            const std::string column{ value };
            basis_database::string_array element{};
            split(element, column, boost::is_any_of("|"), boost::token_compress_off);

            basis_database::int32_array result{};
            for (const auto& value0 : element)
            {
                result.emplace_back(boost::lexical_cast<int32_t>(value0));
            }
            return basis_database{ field_name.get_field_name(), result };
        }

        case database_data_type::int64_array_type:
        {
            const std::string column{ value };
            basis_database::string_array element{};
            split(element, column, boost::is_any_of("|"), boost::token_compress_off);

            basis_database::int64_array result{};
            for (const auto& value0 : element)
            {
                result.emplace_back(boost::lexical_cast<int64_t>(value0));
            }
            return basis_database{ field_name.get_field_name(), result };
        }

        case database_data_type::double_array_type:
        {
            const std::string column{ value };
            basis_database::string_array element{};
            split(element, column, boost::is_any_of("|"), boost::token_compress_off);

            basis_database::double_array result{};
            for (const auto& value0 : element)
            {
                result.emplace_back(boost::lexical_cast<double>(value0));
            }
            return basis_database{ field_name.get_field_name(), element };
        }

        default:
            return basis_database{ field_name.get_field_name(), std::string{} };
    }
}

celeritas::database_session::basis_database_manager_awaitable_type celeritas::redis_database_session::select_one(const basis_database_manager& database, const database_field_container& field_name_container)
{
    redis_hash_commands::field_container field_container{};
    for (const auto& element : field_name_container)
    {
        field_container.emplace_back(element.get_field_name());
    }
    auto result = co_await redis_hash_commands_.async_get_many(generate_key(std::make_shared<basis_database_manager>(database)), field_container);
    basis_database_manager select{ database.get_database_type(), database.get_database_name(), database_change_type::select_type, database.get_key() };

    auto index = 0;
    for (const auto& element : result)
    {
        select.modify(get_basis_database(field_name_container.at(index), element));
        ++index;
    }

    co_return select;
}

celeritas::database_session::result_container_awaitable_type celeritas::redis_database_session::select_all(const basis_database_manager& database, const database_field_container& field_name_container)
{
    const auto result = co_await select_one(database, field_name_container);

    co_return result_container{ result };
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

celeritas::redis_database_session::optional_int_awaitable_type celeritas::redis_database_session::async_execute_command_return_optional_int(const std::string& command) const
{
    check_initialized();

    co_await boost::asio::post(io_context_, boost::asio::use_awaitable);

    const redis_reply redis_reply{ *redis_context_.get(), command };

    co_return redis_reply.to_optional_int();
}

celeritas::redis_database_session::void_awaitable_type celeritas::redis_database_session::save(const basis_database_manager_shared_ptr& database)
{
    if (database->get_change_type() == database_change_type::select_type)
    {
        throw celeritas_error("change type is select.");
    }

    if (database->get_change_type() == database_change_type::delete_type)
    {
        co_await redis_key_commands_.async_delete(generate_key(database));
        co_return;
    }

    redis_commands::key_value_container field_value{};
    for (const auto& element : database->get_database())
    {
        field_value.emplace_back(element.get_field_name(), element.get_string());
    }
    co_await redis_hash_commands_.async_set_many(generate_key(database), field_value);
    co_return;
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




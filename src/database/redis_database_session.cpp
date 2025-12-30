#include "basis_database.tpp"
#include "database_change_type.h"
#include "database_entity_change.h"
#include "database_field.h"
#include "redis_database_session.h"
#include "common/core/celeritas_error.h"
#include "common/common_fwd.h"
#include "common/logger/logger.h"
#include "detail/redis_key_data_converter.h"

using namespace std::literals;

celeritas::redis_database_session::redis_database_session(const std::string_view host,
                                                          const int port,
                                                          const std::string_view user,
                                                          const std::string_view password,
                                                          const std::string_view uri,
                                                          const std::string_view db_name,
                                                          const int expire_seconds,
                                                          io_context_type& io_context)
    : io_context_{ io_context },
      redis_context_{},
      redis_parameter_{ host, port, user, password, db_name, expire_seconds },
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

    redis_context_ = std::make_unique<redis_context>(redis_parameter_.get_host(), redis_parameter_.get_port());

    redis_reply redis_reply{ *redis_context_.get(), redis_parameter_.get_auth_command() };

    LOG_CHANNEL(database_channel, info) << "Authentication successful (AUTH: OK).";

    co_return;
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
    return redis_parameter_.get_prefixed_key(key);
}

celeritas::redis_database_session::array_type celeritas::redis_database_session::get_expire_seconds_command(const int expire_seconds) const
{
    return redis_parameter_.get_expire_seconds_command(expire_seconds);
}

celeritas::redis_database_session::int_awaitable_type celeritas::redis_database_session::async_execute_command_return_int(const array_type& command) const
{
    const auto redis_reply = co_await async_execute_command_return_reply(command);

    co_return redis_reply->to_integer();
}

celeritas::redis_database_session::void_awaitable_type celeritas::redis_database_session::async_execute_command_return_void(const array_type& command) const
{
    const auto redis_reply = co_await async_execute_command_return_reply(command);

    co_return;
}

celeritas::redis_database_session::optional_string_awaitable_type celeritas::redis_database_session::async_execute_command_return_optional_string(const array_type& command) const
{
    const auto redis_reply = co_await async_execute_command_return_reply(command);

    co_return redis_reply->to_optional_string();
}

celeritas::redis_database_session::array_awaitable_type celeritas::redis_database_session::async_execute_command_return_array_type(const array_type& command) const
{
    const auto redis_reply = co_await async_execute_command_return_reply(command);

    co_return redis_reply->to_array();
}

celeritas::redis_database_session::map_awaitable_type celeritas::redis_database_session::async_execute_command_return_map_type(const array_type& command) const
{
    const auto redis_reply = co_await async_execute_command_return_reply(command);

    co_return redis_reply->to_map();
}

celeritas::redis_database_session::optional_double_awaitable_type celeritas::redis_database_session::async_execute_command_return_optional_double(const array_type& command) const
{
    const auto redis_reply = co_await async_execute_command_return_reply(command);

    co_return redis_reply->to_optional_double();
}

celeritas::redis_database_session::optional_int_awaitable_type celeritas::redis_database_session::async_execute_command_return_optional_int(const array_type& command) const
{
    const auto redis_reply = co_await async_execute_command_return_reply(command);

    co_return redis_reply->to_optional_int();
}

celeritas::redis_database_session::optional_map_awaitable_type celeritas::redis_database_session::async_execute_command_return_optional_map_type(const array_type& command) const
{
    const auto redis_reply = co_await async_execute_command_return_reply(command);

    co_return redis_reply->to_optional_map();
}

celeritas::redis_database_session::scan_result_awaitable_type celeritas::redis_database_session::async_execute_command_return_scan_result(const array_type& command) const
{
    const auto redis_reply = co_await async_execute_command_return_reply(command);

    co_return redis_reply->to_scan_result();
}

celeritas::redis_database_session::void_awaitable_type celeritas::redis_database_session::execute_changes(const database_entity_change_const_shared_ptr& database, int expiration_time)
{
    switch (database->get_change_type())
    {
        case database_change_type::select_type:
        {
            throw celeritas_error("change type is select.");
        }

        case database_change_type::update_type:
        case database_change_type::insert_type:
        {
            co_return co_await save_database(database, expiration_time);
        }

        case database_change_type::delete_type:
        {
            co_return co_await delete_database(database);
        }
    }
}

celeritas::database_session::database_entity_change_awaitable_type celeritas::redis_database_session::select_one(const database_entity_change_const_shared_ptr& database, const database_field_container& field_name_container)
{
    const auto optional_result = co_await redis_hash_commands_.async_get_all(redis_key_data_converter::generate_key(database));
    if (!optional_result)
    {
        co_return std::nullopt;
    }

    const auto& result = *optional_result;

    auto select = database->get_select();

    for (const auto& element : field_name_container)
    {
        if (const auto iter = result.find(element.get_field_name().data());
            iter != result.cend())
        {
            select.modify(redis_key_data_converter::get_basis_database(element, iter->second));
        }
        else
        {
            select.modify(redis_key_data_converter::get_basis_database(element));
        }
    }

    co_return select;
}

celeritas::database_session::result_container_awaitable_type celeritas::redis_database_session::select_all(const database_entity_change_const_shared_ptr& database, const database_field_container& field_name_container)
{
    const auto pattern = database->get_database_name().data() + ":*"s;

    const auto keys = co_await redis_key_commands_.async_scan_all(pattern);

    result_container container{};
    for (const auto& element : keys)
    {
        if (const auto select = co_await select_one(element, database, field_name_container))
        {
            container.emplace_back(*select);
        }
    }

    co_return container;
}

void celeritas::redis_database_session::check_initialized() const
{
    if (!redis_context_)
    {
        throw celeritas_error("redis context is not connected or initialized.");
    }
}

void celeritas::redis_database_session::do_is_health() const
{
    check_initialized();

    redis_reply redis_reply{ *redis_context_.get(), "PING" };
}

celeritas::redis_database_session::redis_reply_awaitable_type celeritas::redis_database_session::async_execute_command_return_reply(const array_type& command) const
{
    check_initialized();

    co_await boost::asio::post(io_context_, boost::asio::use_awaitable);

    co_return std::make_unique<redis_reply>(*redis_context_.get(), command);
}

celeritas::redis_database_session::void_awaitable_type celeritas::redis_database_session::save_database(const database_entity_change_const_shared_ptr& database, int expiration_time) const
{
    redis_commands::key_value_container field_value{};
    for (const auto& element : *database->get_database())
    {
        field_value.emplace_back(element.get_field_name(), element.get_string());
    }

    if (expiration_time == 0)
    {
        expiration_time = redis_parameter_.get_expire_seconds();
    }

    const auto key = redis_key_data_converter::generate_key(database);
    co_await redis_hash_commands_.async_set_many(key, field_value);
    co_await redis_key_commands_.async_set_expire_seconds(key, expiration_time);

    co_return;
}

celeritas::redis_database_session::void_awaitable_type celeritas::redis_database_session::delete_database(const database_entity_change_const_shared_ptr& database) const
{
    co_await redis_key_commands_.async_delete(redis_key_data_converter::generate_key(database));
    co_return;
}

celeritas::database_session::database_entity_change_awaitable_type celeritas::redis_database_session::select_one(const std::string& key, const database_entity_change_const_shared_ptr& database, const database_field_container& field_name_container) const
{
    const auto optional_result = co_await redis_hash_commands_.async_get_all(key);
    if (!optional_result)
    {
        co_return std::nullopt;
    }

    const auto& result = *optional_result;

    auto select = database->get_select(redis_key_data_converter::get_key(key, database));

    for (const auto& field : field_name_container)
    {
        if (const auto iter = result.find(field.get_field_name().data());
            iter != result.cend())
        {
            select.modify(redis_key_data_converter::get_basis_database(field, iter->second));
        }
    }

    co_return select;
}




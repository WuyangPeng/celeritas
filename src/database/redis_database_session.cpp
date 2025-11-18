#include "basis_database.tpp"
#include "basis_database_manager.h"
#include "database_change_type.h"
#include "database_data_type.h"
#include "database_field.h"
#include "redis_database_session.h"
#include "common/celeritas_error.h"
#include "common/common_fwd.h"
#include "common/logger.h"

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include <ranges>

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

    const auto command = redis_parameter_.get_auth_command();

    redis_reply redis_reply{ *redis_context_.get(), command };

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

std::string celeritas::redis_database_session::get_expire_seconds_command(const int expire_seconds) const
{
    return redis_parameter_.get_expire_seconds_command(expire_seconds);
}

celeritas::redis_database_session::int_awaitable_type celeritas::redis_database_session::async_execute_command_return_int(const std::string& command) const
{
    const auto redis_reply = co_await async_execute_command_return_reply(command);

    co_return redis_reply.to_integer();
}

celeritas::redis_database_session::void_awaitable_type celeritas::redis_database_session::async_execute_command_return_void(const std::string& command) const
{
    const auto redis_reply = co_await async_execute_command_return_reply(command);

    co_return;
}

celeritas::redis_database_session::optional_string_awaitable_type celeritas::redis_database_session::async_execute_command_return_optional_string(const std::string& command) const
{
    const auto redis_reply = co_await async_execute_command_return_reply(command);

    co_return redis_reply.to_optional_string();
}

celeritas::redis_database_session::array_awaitable_type celeritas::redis_database_session::async_execute_command_return_array_type(const std::string& command) const
{
    const auto redis_reply = co_await async_execute_command_return_reply(command);

    co_return redis_reply.to_array();
}

celeritas::redis_database_session::map_awaitable_type celeritas::redis_database_session::async_execute_command_return_map_type(const std::string& command) const
{
    const auto redis_reply = co_await async_execute_command_return_reply(command);

    co_return redis_reply.to_map();
}

celeritas::redis_database_session::optional_double_awaitable_type celeritas::redis_database_session::async_execute_command_return_optional_double(const std::string& command) const
{
    const auto redis_reply = co_await async_execute_command_return_reply(command);

    co_return redis_reply.to_optional_double();
}

celeritas::redis_database_session::optional_int_awaitable_type celeritas::redis_database_session::async_execute_command_return_optional_int(const std::string& command) const
{
    const auto redis_reply = co_await async_execute_command_return_reply(command);

    co_return redis_reply.to_optional_int();
}

celeritas::redis_database_session::scan_result_awaitable_type celeritas::redis_database_session::async_execute_command_return_scan_result(const std::string& command) const
{
    const auto redis_reply = co_await async_execute_command_return_reply(command);

    co_return redis_reply.to_scan_result();
}

celeritas::redis_database_session::void_awaitable_type celeritas::redis_database_session::execute_changes(const basis_database_manager_const_shared_ptr& database)
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
            co_return co_await save_database(database);
        }

        case database_change_type::delete_type:
        {
            co_return co_await delete_database(database);
        }
    }
}

celeritas::database_session::basis_database_manager_awaitable_type celeritas::redis_database_session::select_one(const basis_database_manager_const_shared_ptr& database, const database_field_container& field_name_container)
{
    const auto result = co_await redis_hash_commands_.async_get_all(generate_key(database));
    auto select = database->get_select();

    for (const auto& element : field_name_container)
    {
        if (const auto iter = result.find(element.get_field_name().data());
            iter != result.cend())
        {
            select.modify(get_basis_database(element, iter->second));
        }
    }

    co_return select;
}

celeritas::database_session::result_container_awaitable_type celeritas::redis_database_session::select_all(const basis_database_manager_const_shared_ptr& database, const database_field_container& field_name_container)
{
    const auto pattern = database->get_database_name().data() + ":*"s;

    const auto keys = co_await redis_key_commands_.async_scan_all(pattern);

    result_container container{};
    for (const auto& element : keys)
    {
        const auto select = co_await select_one(element, database, field_name_container);
        container.emplace_back(select);
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

celeritas::redis_database_session::redis_reply_awaitable_type celeritas::redis_database_session::async_execute_command_return_reply(const std::string& command) const
{
    check_initialized();

    co_await boost::asio::post(io_context_, boost::asio::use_awaitable);

    co_return redis_reply{ *redis_context_.get(), command };
}

celeritas::redis_database_session::void_awaitable_type celeritas::redis_database_session::save_database(const basis_database_manager_const_shared_ptr& database) const
{
    redis_commands::key_value_container field_value{};
    for (const auto& element : database->get_database())
    {
        field_value.emplace_back(element.get_field_name(), element.get_string());
    }

    const auto key = generate_key(database);
    co_await redis_hash_commands_.async_set_many(key, field_value);
    co_await redis_key_commands_.async_set_expire_seconds(key, redis_parameter_.get_expire_seconds());

    co_return;
}

celeritas::redis_database_session::void_awaitable_type celeritas::redis_database_session::delete_database(const basis_database_manager_const_shared_ptr& database) const
{
    co_await redis_key_commands_.async_delete(generate_key(database));
    co_return;
}

celeritas::database_session::basis_database_manager_awaitable_type celeritas::redis_database_session::select_one(const std::string& key, const basis_database_manager_const_shared_ptr& database, const database_field_container& field_name_container) const
{
    const auto result = co_await redis_hash_commands_.async_get_all(key);

    basis_database_manager select{ database->get_database_type(),
                                   database->get_database_name(),
                                   database_change_type::select_type,
                                   get_key(key, database) };

    for (const auto& field : field_name_container)
    {
        if (const auto iter = result.find(field.get_field_name().data());
            iter != result.cend())
        {
            select.modify(get_basis_database(field, iter->second));
        }
    }

    co_return select;
}

celeritas::redis_database_session::array_type celeritas::redis_database_session::get_key_value(const std::string& key)
{
    array_type parts{};
    boost::split(parts, key, boost::is_any_of(":"), boost::token_compress_off);

    if (parts.size() < 2)
    {
        throw celeritas_error("redis key size is error.");
    }

    const auto& key_values = parts.back();

    array_type result{};
    boost::split(result, key_values, boost::is_any_of("_"), boost::token_compress_off);

    return result;
}

std::string celeritas::redis_database_session::generate_key(const basis_database_manager_const_shared_ptr& database)
{
    std::string result{};
    result += database->get_database_name();
    result += ":";

    auto index = 1;
    for (const auto keys = database->get_key();
         const auto& key : keys)
    {
        result += key.get_quotation_mark_string();

        if (index != keys.get_size())
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
            return basis_database{ field_name.get_field_name(), std::stoi(value) };

        case database_data_type::int64_type:
        case database_data_type::int64_count_type:
            return basis_database{ field_name.get_field_name(), std::stoll(value) };

        case database_data_type::double_type:
            return basis_database{ field_name.get_field_name(), std::stod(value) };

        case database_data_type::bool_type:
            return basis_database{ field_name.get_field_name(), value == "true" };

        case database_data_type::string_array_type:
        {
            basis_database::string_array result{};
            split(result, value, boost::is_any_of("|"), boost::token_compress_off);

            return basis_database{ field_name.get_field_name(), result };
        }

        case database_data_type::int32_array_type:
        {
            auto split_view = value | std::views::split('|');

            auto int_view = split_view | std::views::transform([](const auto& subrange) {
                const std::string result{ subrange.begin(), subrange.end() };
                return boost::lexical_cast<int32_t>(result);
            });
            const basis_database::int32_array result{ int_view.begin(), int_view.end() };

            return basis_database{ field_name.get_field_name(), result };
        }

        case database_data_type::int64_array_type:
        {
            auto split_view = value | std::views::split('|');

            auto int_view = split_view | std::views::transform([](const auto& subrange) {
                const std::string result{ subrange.begin(), subrange.end() };
                return boost::lexical_cast<int64_t>(result);
            });
            const basis_database::int64_array result{ int_view.begin(), int_view.end() };

            return basis_database{ field_name.get_field_name(), result };
        }

        case database_data_type::double_array_type:
        {
            auto split_view = value | std::views::split('|');

            auto int_view = split_view | std::views::transform([](const auto& subrange) {
                const std::string result{ subrange.begin(), subrange.end() };
                return boost::lexical_cast<double>(result);
            });
            const basis_database::double_array result{ int_view.begin(), int_view.end() };

            return basis_database{ field_name.get_field_name(), result };
        }

        case database_data_type::byte_array_type:
        {
            const basis_database::byte_array result{ value.cbegin(), value.cend() };

            return basis_database{ field_name.get_field_name(), result };
        }

        default:
            return basis_database{ field_name.get_field_name(), std::string{} };
    }
}

celeritas::basis_database_container celeritas::redis_database_session::get_key(const std::string& key, const basis_database_manager_const_shared_ptr& database)
{
    const auto extracted_key_values = get_key_value(key);

    const auto key_type = database->get_key();
    if (extracted_key_values.size() != key_type.get_size())
    {
        throw celeritas_error("key size is error.");
    }

    basis_database_container::object_container objects{};
    auto index = 0;
    for (const auto& value : key_type)
    {
        objects.emplace_back(value.get_field_name(), extracted_key_values.at(index));
        ++index;
    }

    return basis_database_container{ objects };
}




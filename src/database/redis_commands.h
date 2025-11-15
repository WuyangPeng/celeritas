#pragma once

#include "database_fwd.h"

#include <boost/asio/awaitable.hpp>

#include <string>
#include <vector>

namespace celeritas
{
    class redis_commands
    {
    public:
        using class_type = redis_commands;
        using key_container = std::vector<std::string>;
        using key_value_container = std::vector<std::pair<std::string, std::string> >;

        explicit redis_commands(redis_database_session& session) noexcept;

        virtual ~redis_commands() noexcept = default;

        redis_commands(const redis_commands& rhs) noexcept = default;

        redis_commands& operator=(const redis_commands& rhs) noexcept = delete;

        redis_commands(redis_commands&& rhs) noexcept = default;

        redis_commands& operator=(redis_commands&& rhs) noexcept = delete;

    protected:
        using void_awaitable_type = boost::asio::awaitable<void>;
        using int_awaitable_type = boost::asio::awaitable<int>;
        using optional_string = std::optional<std::string>;
        using optional_string_awaitable_type = boost::asio::awaitable<optional_string>;
        using array_type_awaitable_type = boost::asio::awaitable<key_container>;

        [[nodiscard]] std::string get_keys_command(const key_container& keys) const;

        [[nodiscard]] std::string get_keys_value_command(const key_value_container& key_values) const;

        [[nodiscard]] redis_database_session& get_redis_database_session();

        [[nodiscard]] std::string get_prefixed_key(const std::string& key) const;

        [[nodiscard]] std::string get_expire_seconds_command(int expire_seconds) const;

        [[nodiscard]] int_awaitable_type async_execute_command_return_int(const std::string& command) const;

        [[nodiscard]] void_awaitable_type async_execute_command_return_void(const std::string& command) const;

        [[nodiscard]] optional_string_awaitable_type async_execute_command_return_optional_string(const std::string& command) const;

        [[nodiscard]] array_type_awaitable_type async_execute_command_return_array_type(const std::string& command) const;

    private:
        redis_database_session& session_;
    };
}
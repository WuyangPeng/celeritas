#pragma once

#include "database_fwd.h"
#include "scan_result.h"

#include <boost/asio/awaitable.hpp>

#include <map>
#include <optional>
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
        using array_type = std::vector<std::string>;
        using array_awaitable_type = boost::asio::awaitable<array_type>;
        using map_type = std::map<std::string, std::string>;
        using map_awaitable_type = boost::asio::awaitable<map_type>;
        using optional_double = std::optional<double>;
        using optional_double_awaitable_type = boost::asio::awaitable<optional_double>;
        using optional_int = std::optional<int>;
        using optional_int_awaitable_type = boost::asio::awaitable<optional_int>;
        using string_awaitable_type = boost::asio::awaitable<std::string>;
        using bool_awaitable_type = boost::asio::awaitable<bool>;
        using scan_result_awaitable_type = boost::asio::awaitable<scan_result>;

        [[nodiscard]] std::string get_keys_command(const key_container& keys) const;

        [[nodiscard]] std::string get_fields_command(const key_container& fields) const;

        [[nodiscard]] std::string get_values_command(const key_container& values) const;

        [[nodiscard]] std::string get_keys_value_command(const key_value_container& key_values) const;

        [[nodiscard]] std::string get_fields_value_command(const key_value_container& field_values) const;

        [[nodiscard]] redis_database_session& get_redis_database_session();

        [[nodiscard]] std::string get_prefixed_key(const std::string& key) const;

        [[nodiscard]] std::string get_expire_seconds_command(int expire_seconds) const;

        [[nodiscard]] int_awaitable_type async_execute_command_return_int(const std::string& command) const;

        [[nodiscard]] void_awaitable_type async_execute_command_return_void(const std::string& command) const;

        [[nodiscard]] optional_string_awaitable_type async_execute_command_return_optional_string(const std::string& command) const;

        [[nodiscard]] array_awaitable_type async_execute_command_return_array_type(const std::string& command) const;

        [[nodiscard]] map_awaitable_type async_execute_command_return_map_type(const std::string& command) const;

        [[nodiscard]] optional_double_awaitable_type async_execute_command_return_optional_double(const std::string& command) const;

        [[nodiscard]] optional_int_awaitable_type async_execute_command_return_optional_int(const std::string& command) const;

        [[nodiscard]] scan_result_awaitable_type async_execute_command_return_scan_result(const std::string& command) const;

        [[nodiscard ]] bool_awaitable_type async_execute_command_is_ok(const std::string& command) const;

        [[nodiscard]] std::string get_quoted_value_command(const std::string& value) const;

    private:
        redis_database_session& session_;
    };
}
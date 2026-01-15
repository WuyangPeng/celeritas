#pragma once

#include "database/database_fwd.h"
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
        using bool_awaitable_type = boost::asio::awaitable<bool>;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using int_awaitable_type = boost::asio::awaitable<int>;
        using string_awaitable_type = boost::asio::awaitable<std::string>;
        using optional_int = std::optional<int>;
        using optional_int_awaitable_type = boost::asio::awaitable<optional_int>;
        using optional_double = std::optional<double>;
        using optional_double_awaitable_type = boost::asio::awaitable<optional_double>;
        using optional_string = std::optional<std::string>;
        using optional_string_awaitable_type = boost::asio::awaitable<optional_string>;
        using array_type = std::vector<std::string>;
        using array_awaitable_type = boost::asio::awaitable<array_type>;
        using map_type = std::map<std::string, std::string>;
        using map_awaitable_type = boost::asio::awaitable<map_type>;
        using scan_result_awaitable_type = boost::asio::awaitable<scan_result>;
        using sorted_set_member_score_container = std::vector<sorted_set_member_score>;
        using sorted_set_member_score_awaitable_type = boost::asio::awaitable<sorted_set_member_score_container>;

        [[nodiscard]] array_type get_keys_command(const key_container& keys) const;

        [[nodiscard]] array_type get_keys_value_command(const key_value_container& key_values) const;

        [[nodiscard]] static array_type get_fields_value_command(const key_value_container& field_values);

        [[nodiscard]] redis_database_session& get_redis_database_session();

        [[nodiscard]] std::string get_prefixed_key(const std::string& key) const;

        [[nodiscard]] array_type get_expire_seconds_command(int expire_seconds) const;

        [[nodiscard]] int_awaitable_type async_execute_command_return_int(const array_type& command) const;

        [[nodiscard]] void_awaitable_type async_execute_command_return_void(const array_type& command) const;

        [[nodiscard]] optional_string_awaitable_type async_execute_command_return_optional_string(const array_type& command) const;

        [[nodiscard]] array_awaitable_type async_execute_command_return_array_type(const array_type& command) const;

        [[nodiscard]] map_awaitable_type async_execute_command_return_map_type(const array_type& command) const;

        [[nodiscard]] optional_double_awaitable_type async_execute_command_return_optional_double(const array_type& command) const;

        [[nodiscard]] optional_int_awaitable_type async_execute_command_return_optional_int(const array_type& command) const;

        [[nodiscard]] scan_result_awaitable_type async_execute_command_return_scan_result(const array_type& command) const;

        [[nodiscard]] bool_awaitable_type async_execute_command_is_ok(const array_type& command) const;

        [[nodiscard]] sorted_set_member_score_awaitable_type async_execute_command_return_sorted_set_member_score_type(const array_type& command) const;

    private:
        redis_database_session& session_;
    };
}
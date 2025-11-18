#pragma once

#include "redis_context.h"

#include <map>
#include <optional>
#include <string>
#include <vector>

namespace celeritas
{
    class redis_reply
    {
    public:
        using class_type = redis_reply;
        using optional_string = std::optional<std::string>;
        using array_type = std::vector<std::string>;
        using map_type = std::map<std::string, std::string>;
        using optional_double = std::optional<double>;
        using optional_int = std::optional<int>;

        explicit redis_reply(redis_context& redis_context, const std::string& command);

        ~redis_reply() noexcept;

        redis_reply(const redis_reply& rhs) = delete;

        redis_reply& operator=(const redis_reply& rhs) = delete;

        redis_reply(redis_reply&& rhs) noexcept = default;

        redis_reply& operator=(redis_reply&& rhs) noexcept = default;

        [[nodiscard]] ::redisReply* GetRedisReply() noexcept;

        [[nodiscard]] int to_integer() const;

        [[nodiscard]] optional_int to_optional_int() const;

        [[nodiscard]] optional_double to_optional_double() const;

        [[nodiscard]] optional_string to_optional_string() const;

        [[nodiscard]] array_type to_array() const;

        [[nodiscard]] map_type to_map() const;

    private:
        [[nodiscard]] static std::string to_string_from_element(const ::redisReply* element);

        ::redisReply* redis_reply_;
    };
}
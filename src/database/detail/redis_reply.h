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

        explicit redis_reply(redis_context& redis_context, const std::string& command);

        ~redis_reply() noexcept;

        redis_reply(const redis_reply& rhs) = delete;

        redis_reply& operator=(const redis_reply& rhs) = delete;

        redis_reply(redis_reply&& rhs) noexcept = delete;

        redis_reply& operator=(redis_reply&& rhs) noexcept = delete;

        [[nodiscard]] ::redisReply* GetRedisReply() noexcept;

        [[nodiscard]] int to_integer() const;

        [[nodiscard]] optional_string to_optional_string() const;

        [[nodiscard]] array_type to_array() const;

        [[nodiscard]] map_type to_map() const;

    private:
        ::redisReply* redis_reply_;
    };
}
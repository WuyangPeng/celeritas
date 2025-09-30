#pragma once

#include "redis_context.h"

#include <string>

namespace celeritas
{
    class redis_reply
    {
    public:
        using class_type = redis_reply;

    public:
        explicit redis_reply(redis_context& redis_context, const std::string& command);

        ~redis_reply() noexcept;

        redis_reply(const redis_reply& rhs) = delete;

        redis_reply& operator=(const redis_reply& rhs) = delete;

        redis_reply(redis_reply&& rhs) noexcept = delete;

        redis_reply& operator=(redis_reply&& rhs) noexcept = delete;

        [[nodiscard]] ::redisReply* GetRedisReply() noexcept;

    private:
        ::redisReply* redis_reply_;
    };
}
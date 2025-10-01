#pragma once

#include "hiredis.h"

#include <string>

namespace celeritas
{
    class redis_context
    {
    public:
        using class_type = redis_context;

        redis_context(const std::string& host, int port);

        ~redis_context() noexcept;

        redis_context(const redis_context& rhs) = delete;

        redis_context& operator=(const redis_context& rhs) = delete;

        redis_context(redis_context&& rhs) noexcept = delete;

        redis_context& operator=(redisContext&& rhs) noexcept = delete;

        [[nodiscard]] ::redisContext* get_redis_context() noexcept;

    private:
        ::redisContext* redis_context_;
    };
}


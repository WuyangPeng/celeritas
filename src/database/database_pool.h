#pragma once

#include <boost/asio/awaitable.hpp>

namespace celeritas
{
    class database_pool
    {
    public:
        using class_type = database_pool;
        using awaitable_type = boost::asio::awaitable<void>;

        database_pool() noexcept = default;

        virtual ~database_pool() noexcept = default;

        database_pool(const database_pool& rhs) noexcept = default;

        database_pool& operator=(const database_pool& rhs) noexcept = default;

        database_pool(database_pool&& rhs) noexcept = default;

        database_pool& operator=(database_pool&& rhs) noexcept = default;

        [[nodiscard]] virtual awaitable_type async_initialize() = 0;
    };
}

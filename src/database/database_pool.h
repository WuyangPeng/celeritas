#pragma once

namespace celeritas
{
    class database_pool
    {
    public:
        using class_type = database_pool;

        database_pool() noexcept = default;

        virtual ~database_pool() noexcept = default;

        database_pool(const database_pool& rhs) noexcept = default;

        database_pool& operator=(const database_pool& rhs) noexcept = default;

        database_pool(database_pool&& rhs) noexcept = default;

        database_pool& operator=(database_pool&& rhs) noexcept = default;
    };
}

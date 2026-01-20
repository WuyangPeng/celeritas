#pragma once

#include "database/database_fwd.h"

#include <boost/asio/awaitable.hpp>

#include <memory>

namespace celeritas
{
    class config_table_base
    {
    public:
        using class_type = config_table_base;
        using database_pool_shared_ptr = std::shared_ptr<database_pool_base>;
        using void_awaitable_type = boost::asio::awaitable<void>;

        virtual ~config_table_base() = default;

        [[nodiscard]] virtual void_awaitable_type load(const database_pool_shared_ptr& pool, int64_t id);

        [[nodiscard]] virtual std::string_view get_name() const = 0;

        [[nodiscard]] virtual void_awaitable_type load_all(const database_pool_shared_ptr& pool) = 0;

        [[nodiscard]] virtual void_awaitable_type load_one(const database_pool_shared_ptr& pool, int64_t id) = 0;

        virtual void clear() = 0;
    };
}

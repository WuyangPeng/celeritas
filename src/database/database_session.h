#pragma once

#include "database_fwd.h"

#include <boost/asio/awaitable.hpp>
#include <chrono>

namespace celeritas
{
    // 数据库会话类，代表一个数据库连接
    class database_session
    {
    public:
        using class_type = database_session;
        using time_point_type = std::chrono::steady_clock::time_point;
        using bool_awaitable_type = boost::asio::awaitable<bool>;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using basis_database_manager_shared_ptr = std::shared_ptr<basis_database_manager>;

        database_session() noexcept = default;

        virtual ~database_session() noexcept = default;

        database_session(const database_session& rhs) noexcept = default;

        database_session& operator=(const database_session& rhs) noexcept = default;

        database_session(database_session&& rhs) noexcept = default;

        database_session& operator=(database_session&& rhs) noexcept = default;

        [[nodiscard]] bool is_expired() const;

        void set_last_heartbeat();

        [[nodiscard]] virtual bool_awaitable_type is_health() = 0;

        [[nodiscard]] virtual void_awaitable_type save(const basis_database_manager_shared_ptr& database) = 0;

    private:
        time_point_type last_heartbeat;
    };
}

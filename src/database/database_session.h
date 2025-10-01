#pragma once

#include <chrono>

namespace celeritas
{
    // 数据库会话类，代表一个数据库连接
    class database_session
    {
    public:
        using class_type = database_session;
        using time_point_type = std::chrono::steady_clock::time_point;

        database_session() noexcept = default;

        virtual ~database_session() noexcept = default;

        database_session(const database_session& rhs) noexcept = default;

        database_session& operator=(const database_session& rhs) noexcept = default;

        database_session(database_session&& rhs) noexcept = default;

        database_session& operator=(database_session&& rhs) noexcept = default;

        [[nodiscard]] bool is_expired() const;

        void set_last_heartbeat();

    private:
        time_point_type last_heartbeat;
    };
}

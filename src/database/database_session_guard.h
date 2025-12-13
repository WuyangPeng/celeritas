#pragma once

#include "connection_pool.h"

#include <memory>

namespace celeritas
{
    template <typename SessionType>
    class database_session_guard
    {
    public:
        using class_type = database_session_guard;
        using session_shared_ptr = std::shared_ptr<SessionType>;
        using connection_pool_shared_ptr = std::shared_ptr<connection_pool<SessionType> >;

        database_session_guard(const session_shared_ptr& session, const connection_pool_shared_ptr& pool);

        ~database_session_guard() noexcept;

        database_session_guard(const database_session_guard& rhs) = delete;

        database_session_guard& operator=(const database_session_guard& rhs) = delete;

        database_session_guard(database_session_guard&& rhs) noexcept;

        database_session_guard& operator=(database_session_guard&& rhs) noexcept;

        [[nodiscard]] session_shared_ptr get_session();

    private:
        session_shared_ptr session_;
        connection_pool_shared_ptr pool_;
    };
}
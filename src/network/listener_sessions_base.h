#pragma once

#include <memory>

namespace celeritas
{
    class listener_sessions_base : public std::enable_shared_from_this<listener_sessions_base>
    {
    public:
        using class_type = listener_sessions_base;

        listener_sessions_base() noexcept = default;

        virtual ~listener_sessions_base() noexcept = default;

        listener_sessions_base(const listener_sessions_base& rhs) = delete;

        listener_sessions_base& operator=(const listener_sessions_base& rhs) = delete;

        listener_sessions_base(listener_sessions_base&& rhs) noexcept = delete;

        listener_sessions_base& operator=(listener_sessions_base&& rhs) noexcept = delete;

        virtual void remove_session(int64_t session_id) = 0;
    };
}
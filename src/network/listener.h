#pragma once

#include <memory>

namespace celeritas
{
    class listener : public std::enable_shared_from_this<listener>
    {
    public:
        using class_type = listener;

        listener() noexcept = default;

        virtual ~listener() noexcept = default;

        listener(const listener& rhs) = delete;

        listener& operator=(const listener& rhs) = delete;

        listener(listener&& rhs) noexcept = delete;

        listener& operator=(listener&& rhs) noexcept = delete;

        virtual void remove_session(long session_id) = 0;

        virtual void stop() = 0;
    };
}


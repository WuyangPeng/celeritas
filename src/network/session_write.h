#pragma once

#include "common/buffer_guard.h"

namespace celeritas
{
    class session_write
    {
    public:
        using class_type = session_write;

        session_write() noexcept = default;

        virtual ~session_write() noexcept = default;

        session_write(const session_write& rhs) = default;

        session_write& operator=(const session_write& rhs) = default;

        session_write(session_write&& rhs) noexcept = default;

        session_write& operator=(session_write&& rhs) noexcept = default;

        // 向客户端发送消息
        virtual void write(buffer_guard data) = 0;
    };
}
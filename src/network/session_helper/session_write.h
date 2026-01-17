#pragma once

#include "common/common_fwd.h"

#include <boost/asio/awaitable.hpp>

namespace celeritas
{
    class session_write : public std::enable_shared_from_this<session_write>
    {
    public:
        using class_type = session_write;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using session_weak_ptr = std::weak_ptr<session>;

        session_write() noexcept = default;

        virtual ~session_write() noexcept = default;

        session_write(const session_write& rhs) = default;

        session_write& operator=(const session_write& rhs) = default;

        session_write(session_write&& rhs) noexcept = default;

        session_write& operator=(session_write&& rhs) noexcept = default;

        // 向客户端发送消息
        virtual void write(buffer_guard data) = 0;

        [[nodiscard]] virtual void_awaitable_type write_immediately(buffer_guard data, const session_weak_ptr& session) = 0;

        // 协程：处理发送队列
        [[nodiscard]] virtual void_awaitable_type do_write() = 0;

        [[nodiscard]] virtual bool is_full() = 0;
    };
}
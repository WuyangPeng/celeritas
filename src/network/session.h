#pragma once

#include <boost/asio/awaitable.hpp>
#include <memory>

namespace celeritas
{
    class session : public std::enable_shared_from_this<session>
    {
    public:
        using class_type = session;
        using void_awaitable_type = boost::asio::awaitable<void>;

        explicit session(int64_t session_id) noexcept;

        virtual ~session() noexcept = default;

        session(const session& rhs) = default;

        session& operator=(const session& rhs) = default;

        session(session&& rhs) noexcept = default;

        session& operator=(session&& rhs) noexcept = default;

        // 启动会话处理协程
        virtual void start() = 0;

        // 协程：处理会话的读写循环
        [[nodiscard]] virtual void_awaitable_type run() = 0;

        [[nodiscard]] int64_t get_session_id() const noexcept;

    private:
        int64_t session_id_;
    };
}
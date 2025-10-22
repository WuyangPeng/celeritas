#pragma once

#include "network/session_write.h"

#include <deque>
#include <optional>

namespace celeritas
{
    template <typename SocketType>
    class tcp_session_write final : public session_write
    {
    public:
        using class_type = tcp_session_write;
        using base_type = session_write;
        using socket_type = SocketType;

        explicit tcp_session_write(socket_type& socket);

        ~tcp_session_write() noexcept override = default;

        tcp_session_write(const tcp_session_write& rhs) = delete;

        tcp_session_write& operator=(const tcp_session_write& rhs) = delete;

        tcp_session_write(tcp_session_write&& rhs) noexcept = delete;

        tcp_session_write& operator=(tcp_session_write&& rhs) noexcept = delete;

        void write(buffer_guard data) override;

        // 协程：处理发送队列
        [[nodiscard]] void_awaitable_type do_write() override;

    private:
        using buffer_guard_optional_type = std::optional<buffer_guard>;

        // 协程：处理单个写入操作
        [[nodiscard]] void_awaitable_type do_one_write();

        // 从发送队列中获取下一个缓冲区，并在加锁后立即释放锁
        [[nodiscard]] buffer_guard_optional_type get_next_write_buffer();

        socket_type& socket_;
        std::deque<buffer_guard> write_queue_;
        std::mutex write_mutex_;
    };
}
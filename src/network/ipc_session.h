#pragma once

#include "common/buffer_guard.h"
#include "common/common_fwd.h"
#include "network_fwd.h"

#include <boost/asio/awaitable.hpp>
#include <boost/asio/local/stream_protocol.hpp>

#include <deque>

namespace celeritas
{
    class ipc_session : public std::enable_shared_from_this<ipc_session>
    {
    public:
        using class_type = ipc_session;
        using socket_type = boost::asio::local::stream_protocol::socket;
        using awaitable_type = boost::asio::awaitable<void>;
        using message_handler_type = std::function<void(const message_header&, buffer_guard)>;

        ipc_session(socket_type socket, message_handler_type handler);

        void start();

        void write(buffer_guard data);

    private:
        using buffer_guard_optional_type = std::optional<buffer_guard>;

        [[nodiscard]] awaitable_type handle_read();
        [[nodiscard]] awaitable_type handle_one_message();

        // 处理发送队列的协程
        [[nodiscard]] awaitable_type do_write();

        // 处理单个写入操作的协程
        [[nodiscard]] awaitable_type do_one_write();

        // 从发送队列中获取下一个缓冲区的函数
        [[nodiscard]] buffer_guard_optional_type get_next_write_buffer();

        socket_type socket_;
        message_handler_type message_handler_;

        // 发送队列和互斥锁
        std::deque<buffer_guard> write_queue_;
        std::mutex write_mutex_;
    };
}

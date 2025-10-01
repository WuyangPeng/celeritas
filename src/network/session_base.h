#pragma once

#include "network_message_callback.h"
#include "common/buffer_guard.h"

#include <boost/asio.hpp>
#include <deque>

namespace celeritas
{
    template <typename SocketType>
    class session_base : public std::enable_shared_from_this<session_base<SocketType> >
    {
    public:
        using class_type = session_base;
        using socket_type = SocketType;

        using network_message_callback_shared_ptr = std::shared_ptr<network_message_callback>;

        // 构造函数：接受一个已连接的 socket
        explicit session_base(socket_type socket, const network_message_callback_shared_ptr& callback);

        ~session_base() noexcept = default;

        session_base(const session_base& rhs) = delete;

        session_base& operator=(const session_base& rhs) = delete;

        session_base(session_base&& rhs) noexcept = delete;

        session_base& operator=(session_base&& rhs) noexcept = delete;

        // 启动会话处理协程
        void start();

        // 向客户端发送消息
        void write(buffer_guard data);

    private:
        using void_awaitable_type = boost::asio::awaitable<void>;
        using read_awaitable_type = boost::asio::awaitable<size_t>;
        using buffer_guard_optional_type = std::optional<buffer_guard>;

        // 协程：处理带超时的异步读取操作
        [[nodiscard]] read_awaitable_type read_data_with_timeout(boost::asio::mutable_buffer buffer);

        // 协程：处理会话的读写循环
        [[nodiscard]] void_awaitable_type handle_session();

        [[nodiscard]] void_awaitable_type handle_one_message();

        // 协程：处理发送队列
        [[nodiscard]] void_awaitable_type do_write();

        // 协程：处理单个写入操作
        [[nodiscard]] void_awaitable_type do_one_write();

        // 从发送队列中获取下一个缓冲区，并在加锁后立即释放锁
        [[nodiscard]] buffer_guard_optional_type get_next_write_buffer();

        socket_type socket_;
        network_message_callback_shared_ptr network_message_callback_;

        // 发送队列和互斥锁
        std::deque<buffer_guard> write_queue_;
        std::mutex write_mutex_;
    };
}

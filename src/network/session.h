#pragma once

#include "common/buffer_guard.h"
#include "network_fwd.h"

#include <boost/asio.hpp>
#include <deque>

namespace celeritas
{
    class session : public std::enable_shared_from_this<session>
    {
    public:
        using class_type = session;
        using socket_type = boost::asio::ip::tcp::socket;

        // 定义消息处理回调函数的类型
        // 参数为消息头和消息体的缓冲区，返回void
        using message_handler_type = std::function<void(const message_header&, buffer_guard)>;

        // 构造函数：接受一个已连接的 socket
        explicit session(socket_type socket, message_handler_type handler);

        // 启动会话处理协程
        void start();

        // 向客户端发送消息
        void write(buffer_guard data);

    private:
        using awaitable_type = boost::asio::awaitable<void>;
        using read_awaitable_type = boost::asio::awaitable<size_t>;
        using buffer_guard_optional_type = std::optional<buffer_guard>;

        // 协程：处理带超时的异步读取操作
        [[nodiscard]] read_awaitable_type read_data_with_timeout(boost::asio::mutable_buffer buffer);

        // 协程：处理会话的读写循环
        [[nodiscard]] awaitable_type handle_session();
        [[nodiscard]] awaitable_type handle_one_message();

        // 协程：处理发送队列
        [[nodiscard]] awaitable_type do_write();

        // 协程：处理单个写入操作
        [[nodiscard]] awaitable_type do_one_write();

        // 从发送队列中获取下一个缓冲区，并在加锁后立即释放锁
        [[nodiscard]] buffer_guard_optional_type get_next_write_buffer();

        socket_type socket_;
        message_handler_type message_handler_;

        // 发送队列和互斥锁
        std::deque<buffer_guard> write_queue_;
        std::mutex write_mutex_;
    };
}

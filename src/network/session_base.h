#pragma once

#include "listener.h"
#include "network_message_callback.h"
#include "session.h"
#include "common/buffer_guard.h"

#include <boost/asio.hpp>
#include <deque>

namespace celeritas
{
    template <typename SocketType>
    class session_base : public session
    {
    public:
        using class_type = session_base;
        using socket_type = SocketType;

        using network_message_callback_weak_ptr = std::weak_ptr<network_message_callback>;
        using listener_shared_ptr = std::shared_ptr<listener>;

        // 构造函数：接受一个已连接的 socket
        explicit session_base(socket_type socket,
                              long session_id,
                              const std::string& game_server_id,
                              session_callback session_callback);

        ~session_base() noexcept = default;

        session_base(const session_base& rhs) = delete;

        session_base& operator=(const session_base& rhs) = delete;

        session_base(session_base&& rhs) noexcept = delete;

        session_base& operator=(session_base&& rhs) noexcept = delete;

        // 启动会话处理协程
        void start() override;

        // 向客户端发送消息
        void write(buffer_guard data) override;

    private:
        using void_awaitable_type = boost::asio::awaitable<void>;
        using read_awaitable_type = boost::asio::awaitable<size_t>;
        using buffer_guard_optional_type = std::optional<buffer_guard>;
        using listener_weak_ptr = std::weak_ptr<listener>;
        using session_write_shared_ptr = std::shared_ptr<session_write>;

        // 协程：处理带超时的异步读取操作
        [[nodiscard]] read_awaitable_type read_data_with_timeout(boost::asio::mutable_buffer buffer);

        // 协程：处理会话的读写循环
        [[nodiscard]] void_awaitable_type run();

        [[nodiscard]] void_awaitable_type handle_one_message();

        socket_type socket_;
        session_write_shared_ptr session_write_;
        std::string game_server_id_;
    };
}

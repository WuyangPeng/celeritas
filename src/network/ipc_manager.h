#pragma once

#include "message_header.h"
#include "session_base.h"

#include <boost/asio.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/local/stream_protocol.hpp>
#include <boost/asio/use_awaitable.hpp>

namespace celeritas
{
    class ipc_manager
    {
    public:
        using class_type = session_base<boost::asio::local::stream_protocol::socket>;
        using message_handler_type = std::function<void(const message_header&, buffer_guard)>;

        // 构造函数用于作为服务器监听连接
        ipc_manager(boost::asio::io_context& io_context, const std::string& socket_path, message_handler_type handler);

        // 接受 IPC 连接
        [[nodiscard]] boost::asio::awaitable<void> accept_connections();

        // 作为客户端连接到另一个进程
        [[nodiscard]] boost::asio::awaitable<std::shared_ptr<session_base<boost::asio::local::stream_protocol::socket>>> connect_to(const std::string& remote_path);

    private:
        boost::asio::io_context& io_context_;
        boost::asio::local::stream_protocol::acceptor acceptor_;
        message_handler_type message_handler_;
    };
}

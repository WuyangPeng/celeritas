#pragma once

#include <boost/asio.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/asio/local/stream_protocol.hpp>

namespace celeritas
{
    class ipc_session;

    class ipc_manager
    {
    public:
        // 构造函数用于作为服务器监听连接
        ipc_manager(boost::asio::io_context& io_context, const std::string& socket_path);

        // 接受 IPC 连接
        [[nodiscard]] boost::asio::awaitable<void> accept_connections();

        // 作为客户端连接到另一个进程
        [[nodiscard]] boost::asio::awaitable<std::shared_ptr<ipc_session>> connect_to(const std::string& remote_path);

    private:
        boost::asio::io_context& io_context_;
        boost::asio::local::stream_protocol::acceptor acceptor_;
    };
}




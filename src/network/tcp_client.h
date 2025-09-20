#pragma once

#include "message_header.h"
#include "session_base.h"

#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>
#include <functional>

namespace celeritas
{
    class tcp_client
    {
    public:
        using class_type = tcp_client;
        using socket_type = boost::asio::ip::tcp::socket;
        using session_type = session_base<socket_type>;
        using message_handler_type = session_type::message_handler_type;
        using session_waitable_type = boost::asio::awaitable<std::shared_ptr<session_type> >;

        // 构造函数：接受 io_context 和消息处理回调
        tcp_client(boost::asio::io_context& io_context, message_handler_type handler);

        ~tcp_client() noexcept = default;

        tcp_client(const tcp_client& rhs) = delete;

        tcp_client& operator=(const tcp_client& rhs) = delete;

        tcp_client(tcp_client&& rhs) noexcept = delete;

        tcp_client& operator=(tcp_client&& rhs) noexcept = delete;

        // 异步连接到指定的远程地址和端口
        [[nodiscard]] session_waitable_type connect(const std::string& host, uint16_t port);

    private:
        boost::asio::io_context& io_context_;
        message_handler_type message_handler_;
    };
}
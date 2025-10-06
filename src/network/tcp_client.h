#pragma once

#include "message_header.h"
#include "session_base.h"

#include <boost/asio/awaitable.hpp>
#include <functional>

namespace celeritas
{
    class tcp_client : public listener
    {
    public:
        using class_type = tcp_client;
        using base_type = listener;

        using socket_type = boost::asio::ip::tcp::socket;
        using session_type = session_base<socket_type>;
        using network_message_callback_shared_ptr = std::shared_ptr<network_message_callback>;
        using session_waitable_type = boost::asio::awaitable<std::shared_ptr<session_type> >;

        // 构造函数：接受 io_context 和消息处理回调
        tcp_client(boost::asio::io_context& io_context, const network_message_callback_shared_ptr& callback);

        ~tcp_client() noexcept override = default;

        tcp_client(const tcp_client& rhs) = delete;

        tcp_client& operator=(const tcp_client& rhs) = delete;

        tcp_client(tcp_client&& rhs) noexcept = delete;

        tcp_client& operator=(tcp_client&& rhs) noexcept = delete;

        // 异步连接到指定的远程地址和端口
        [[nodiscard]] session_waitable_type connect(const std::string& host, uint16_t port);

    private:
        boost::asio::io_context& io_context_;
        network_message_callback_shared_ptr callback_;
        long session_id_;
    };
}
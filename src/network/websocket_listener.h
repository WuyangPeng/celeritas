#pragma once

#include "listener.h"
#include "websocket_session.h"

#include <boost/asio.hpp>
#include <map>

namespace celeritas
{
    class websocket_listener : public listener
    {
    public:
        using class_type = websocket_listener;
        using base_type = listener;
        using session_type = websocket_session; // 使用专用的 websocket_session
        using network_message_callback_shared_ptr = std::shared_ptr<network_message_callback>;

        // 接受 io_context 和监听端口
        websocket_listener(boost::asio::io_context& io_context,
                           int port,
                           const network_message_callback_shared_ptr& callback);

        ~websocket_listener() noexcept override = default;

        websocket_listener(const websocket_listener& rhs) = delete;

        websocket_listener& operator=(const websocket_listener& rhs) = delete;

        websocket_listener(websocket_listener&& rhs) noexcept = delete;

        websocket_listener& operator=(websocket_listener&& rhs) noexcept = delete;

        // 开始监听新连接
        void start();

        // 停止监听器
        void stop();

    private:
        using io_context_type = boost::asio::io_context;
        using acceptor_type = boost::asio::ip::tcp::acceptor;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using session_shared_ptr = std::shared_ptr<session_type>;
        using session_type_container_type = std::map<long, session_shared_ptr>;

        // 协程：异步接受新连接
        [[nodiscard]] void_awaitable_type accept_connections();

        // 协程：处理单个连接
        [[nodiscard]] void_awaitable_type handle_connection();

        io_context_type& io_context_;
        acceptor_type acceptor_;
        network_message_callback_shared_ptr network_message_callback_;
        bool is_running_;
        session_type_container_type sessions_;
        long session_id_;
    };
}
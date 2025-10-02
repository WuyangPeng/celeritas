#pragma once

#include "listener.h"
#include "session_base.h"

#include <boost/asio.hpp>
#include <map>

namespace celeritas
{
    class tcp_listener : public listener
    {
    public:
        using class_type = tcp_listener;
        using base_type = listener;
        using session_type = session_base<boost::asio::ip::tcp::socket>;
        using network_message_callback_shared_ptr = std::shared_ptr<network_message_callback>;

        // 接受 io_context和监听端口
        tcp_listener(boost::asio::io_context& io_context, int port, const network_message_callback_shared_ptr& callback);

        ~tcp_listener() noexcept override = default;

        tcp_listener(const tcp_listener& rhs) = delete;

        tcp_listener& operator=(const tcp_listener& rhs) = delete;

        tcp_listener(tcp_listener&& rhs) noexcept = delete;

        tcp_listener& operator=(tcp_listener&& rhs) noexcept = delete;

        // 开始监听新连接
        void start();

        // 停止监听器
        void stop() override;

        void remove_session(long session_id) override;

    private:
        using io_context_type = boost::asio::io_context;
        using acceptor_type = boost::asio::ip::tcp::acceptor;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using session_shared_ptr = std::shared_ptr<session_type>;
        using session_type_container_type = std::map<long, session_shared_ptr>;

        // 协程：异步接受新连接
        [[nodiscard]] void_awaitable_type accept_connections();

        [[nodiscard]] void_awaitable_type handle_connection();

        io_context_type& io_context_;
        acceptor_type acceptor_;
        network_message_callback_shared_ptr network_message_callback_;
        std::atomic<bool> is_running_;
        session_type_container_type sessions_;
        long session_id_;
    };
}

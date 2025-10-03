#pragma once

#include "http_session.h"
#include "listener.h"

#include <map>

namespace celeritas
{
    class http_listener : public listener
    {
    public:
        using class_type = http_listener;
        using base_type = listener;
        using session_type = http_session; // 使用专用的 http_session
        using network_message_callback_weak_ptr = std::weak_ptr<network_message_callback>;

        // 接受 io_context 和监听端口
        http_listener(boost::asio::io_context& io_context,
                      int port,
                      const network_message_callback_weak_ptr& callback);

        ~http_listener() noexcept override = default;

        http_listener(const http_listener& rhs) = delete;

        http_listener& operator=(const http_listener& rhs) = delete;

        http_listener(http_listener&& rhs) noexcept = delete;

        http_listener& operator=(http_listener&& rhs) noexcept = delete;

        // 停止监听器
        void stop() override;

        void remove_session(int64_t session_id) override;

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
        network_message_callback_weak_ptr network_message_callback_;
        bool is_running_;
        session_type_container_type sessions_;
        long session_id_;
    };
}
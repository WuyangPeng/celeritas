#pragma once

#include "listener.h"
#include "session_base.h"
#include "session_listener.h"

#include <boost/asio.hpp>

namespace celeritas
{
    class tcp_listener : public session_listener
    {
    public:
        using class_type = tcp_listener;
        using base_type = session_listener;
        using session_type = session_base<boost::asio::ip::tcp::socket>;

        tcp_listener(io_context_type& io_context,
                     network_message_callback_weak_ptr callback,
                     std::string game_server_id,
                     int port);

        ~tcp_listener() noexcept override = default;

        tcp_listener(const tcp_listener& rhs) = delete;

        tcp_listener& operator=(const tcp_listener& rhs) = delete;

        tcp_listener(tcp_listener&& rhs) noexcept = delete;

        tcp_listener& operator=(tcp_listener&& rhs) noexcept = delete;

        // 停止监听器
        void stop() override;

        // 协程：异步接受新连接
        [[nodiscard]] void_awaitable_type accept_connections() override;

    private:
        using acceptor_type = boost::asio::ip::tcp::acceptor;

        [[nodiscard]] void_awaitable_type handle_connection();

        acceptor_type acceptor_;
    };
}

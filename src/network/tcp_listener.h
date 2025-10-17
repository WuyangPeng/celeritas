#pragma once

#include "listener.h"
#include "listener_accept.h"

namespace celeritas
{
    class tcp_listener final : public listener
    {
    public:
        using class_type = tcp_listener;
        using base_type = listener;

        tcp_listener(io_context_type& io_context,
                     network_message_callback_weak_ptr callback,
                     std::string game_server_id,
                     int port);

        // 停止监听器
        void stop() override;

        // 协程：异步接受新连接
        [[nodiscard]] void_awaitable_type accept_connections() override;

    private:
        using acceptor_type = boost::asio::ip::tcp::acceptor;
        using listener_accept_shared_ptr = std::shared_ptr<listener_accept>;

        acceptor_type acceptor_;
        listener_accept_shared_ptr listener_accept_;
    };
}

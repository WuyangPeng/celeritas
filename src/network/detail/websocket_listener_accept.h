#pragma once

#include "network/listener_accept.h"
#include "network/websocket_session.h"

#include <boost/asio/ip/tcp.hpp>

namespace celeritas
{
    class websocket_listener_accept final : public listener_accept
    {
    public:
        using class_type = websocket_listener_accept;
        using base_type = listener_accept;
        using acceptor_type = boost::asio::ip::tcp::acceptor;

        websocket_listener_accept(acceptor_type& acceptor, server_network_type server_network_type, std::string game_server_id, network_message_callback_weak_ptr callback);

        // 停止监听器
        void stop() override;

        // 协程：异步接受新连接
        [[nodiscard]] void_awaitable_type accept_connections() override;

    private:
        using socket_type = websocket_session::socket_type;

        [[nodiscard]] void_awaitable_type do_accept_connections();

        // 协程：处理单个连接
        [[nodiscard]] void_awaitable_type handle_connection();

        void start_new_session(socket_type socket);

        acceptor_type& acceptor_;
        std::string game_server_id_;
        network_message_callback_weak_ptr callback_;
    };
}
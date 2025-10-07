#pragma once

#include "boost/asio/ip/tcp.hpp"
#include "network/listener_accept.h"
#include "network/websocket_session.h"

namespace celeritas
{
    class websocket_listener_accept final : public listener_accept
    {
    public:
        using class_type = websocket_listener_accept;
        using base_type = listener_accept;
        using acceptor_type = boost::asio::ip::tcp::acceptor;

        websocket_listener_accept(acceptor_type& acceptor, std::string game_server_id);

        // 停止监听器
        void stop() override;

        // 协程：异步接受新连接
        [[nodiscard]] void_awaitable_type accept_connections() override;

    private:
        using socket_type = websocket_session::socket_type;

        // 协程：处理单个连接
        [[nodiscard]] void_awaitable_type handle_connection();

        void start_new_session(socket_type socket);

        acceptor_type& acceptor_;
        std::string game_server_id_;
        session_callback session_callback_;
    };
}
#pragma once

#include "network/listener_accept.h"

#include <boost/asio.hpp>

namespace celeritas
{
    class http_listener_accept : public listener_accept
    {
    public:
        using class_type = http_listener_accept;
        using base_type = listener_accept;
        using acceptor_type = boost::asio::ip::tcp::acceptor;

        http_listener_accept(acceptor_type& acceptor, std::string game_server_id, network_message_callback_weak_ptr callback);

        // 停止监听器
        void stop() override;

        // 协程：异步接受新连接
        [[nodiscard]] void_awaitable_type accept_connections() override;

    private:
        using socket_type = boost::asio::ip::tcp::socket;

        // 协程：处理单个连接
        [[nodiscard]] void_awaitable_type handle_connection();

        void start_new_session(socket_type socket);

        acceptor_type& acceptor_;
        std::string game_server_id_;
        network_message_callback_weak_ptr callback_;
    };
}

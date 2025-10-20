#pragma once

#include "message_header.h"
#include "session_base.h"
#include "proto/request.pb.h"

#include <boost/asio/awaitable.hpp>

namespace celeritas
{
    class tcp_client final : public listener_sessions
    {
    public:
        using class_type = tcp_client;
        using base_type = listener_sessions;

        using io_context_type = boost::asio::io_context;
        using socket_type = boost::asio::ip::tcp::socket;
        using session_type = session_base<socket_type>;
        using session_type_shared_type = std::shared_ptr<session_type>;
        using session_waitable_type = boost::asio::awaitable<session_type_shared_type>;

        tcp_client(io_context_type& io_context,
                   network_message_callback_weak_ptr callback,
                   std::string game_server_id,
                   std::string host,
                   int port,
                   std::string server_type);

        // 异步连接到指定的远程地址和端口
        [[nodiscard]] session_waitable_type connect();

        [[nodiscard]] bool is_open() const;

        [[nodiscard]] std::string get_host() const;

        [[nodiscard]] int get_port() const;

        [[nodiscard]] std::string get_server_type() const;

        [[nodiscard]] network_message_callback_weak_ptr get_network_message_callback();

        void write(const header& header, const google::protobuf::Message& request);

    private:
        io_context_type& io_context_;
        network_message_callback_weak_ptr network_message_callback_;
        std::string game_server_id_;
        long session_id_;
        session_type_shared_type session_;
        std::string host_;
        int port_;
        std::string server_type_;
    };
}
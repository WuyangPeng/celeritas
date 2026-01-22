#pragma once

#include "network/session/generic_session.h"
#include "proto/celeritas.pb.h"

#include <boost/asio/awaitable.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace celeritas
{
    class tcp_client final : public listener_sessions_base
    {
    public:
        using class_type = tcp_client;
        using base_type = listener_sessions_base;

        using io_context_type = boost::asio::io_context;
        using tcp_type = boost::asio::ip::tcp;
        using socket_type = tcp_type::socket;
        using session_type = generic_session<socket_type>;
        using session_type_shared_type = std::shared_ptr<session_type>;
        using session_waitable_type = boost::asio::awaitable<session_type_shared_type>;
        using void_waitable_type = boost::asio::awaitable<void>;
        using protobuf_message = google::protobuf::Message;
        using network_message_callback_weak_ptr = std::weak_ptr<network_message_callback>;
        using any_io_executor = boost::asio::any_io_executor;

        tcp_client(const any_io_executor& any_io_executor,
                   network_message_callback_weak_ptr callback,
                   std::string game_server_id,
                   std::string instance_id,
                   std::string host,
                   int port,
                   std::string server_type);

        // 异步连接到指定的远程地址和端口
        [[nodiscard]] session_waitable_type connect();

        [[nodiscard]] std::string get_host() const;

        [[nodiscard]] int get_port() const;

        [[nodiscard]] std::string get_server_type() const;

        [[nodiscard]] std::string get_game_server_id() const;

        [[nodiscard]] std::string get_instance_id() const;

        [[nodiscard]] network_message_callback_weak_ptr get_network_message_callback();

        void write(const header& header, const protobuf_message& request) const;

        [[nodiscard]] bool is_open() const;

        void remove_session(int64_t session_id) override;

        void stop();

        [[nodiscard]] bool is_full() const;

    private:
        using basic_resolver_results_type = boost::asio::ip::basic_resolver_results<tcp_type>;
        using basic_resolver_results_waitable_type = boost::asio::awaitable<basic_resolver_results_type>;

        [[nodiscard]] basic_resolver_results_waitable_type get_end_points() const;

        [[nodiscard]] void_waitable_type do_connect();

        any_io_executor any_io_executor_;
        network_message_callback_weak_ptr network_message_callback_;
        std::string game_server_id_;
        std::string instance_id_;
        std::string host_;
        int port_;
        std::string server_type_;
        session_type_shared_type session_;

        static constexpr auto session_id_ = 1;
    };
}
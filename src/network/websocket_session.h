#pragma once

#include "network_fwd.h"
#include "session_base.h"

#include <boost/asio.hpp>
#include <boost/beast.hpp>

namespace celeritas
{
    namespace beast_websocket = boost::beast::websocket;

    class websocket_session final : public session_base
    {
    public:
        using class_type = websocket_session;
        using base_type = session_base;
        using socket_type = boost::asio::ip::tcp::socket;

        websocket_session(socket_type socket,
                          server_network_type server_network_type,
                          int64_t session_id,
                          const std::string& game_server_id,
                          session_callback session_callback);

        ~websocket_session() noexcept override;

        websocket_session(const websocket_session& rhs) = delete;

        websocket_session& operator=(const websocket_session& rhs) = delete;

        websocket_session(websocket_session&& rhs) noexcept = delete;

        websocket_session& operator=(websocket_session&& rhs) noexcept = delete;

        // 启动会话处理协程
        void start() override;

        [[nodiscard]] void_awaitable_type start_awaitable() override;

        void stop() override;

        [[nodiscard]] bool is_open() const override;

        [[nodiscard]] bool is_full() const override;

    private:
        // WebSocket 流的类型
        using tcp_stream_type = boost::beast::tcp_stream;
        using web_socket_stream_type = beast_websocket::stream<tcp_stream_type>;

        using session_run_shared_ptr = std::shared_ptr<session_run>;
        using session_write_shared_ptr = std::shared_ptr<session_write>;

        void set_option(const std::string& game_server_id);

        void do_write(buffer_guard data) override;

        [[nodiscard]] void_awaitable_type do_write_immediately(buffer_guard data) override;

        web_socket_stream_type websocket_;
        session_run_shared_ptr websocket_run_;
        session_write_shared_ptr websocket_write_;
    };
}


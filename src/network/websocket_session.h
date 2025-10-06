#pragma once

#include "session.h"
#include "network_fwd.h"

#include <boost/asio.hpp>
#include <boost/beast.hpp>

namespace celeritas
{
    namespace beast_websocket = boost::beast::websocket;

    class websocket_session final : public session
    {
    public:
        using class_type = websocket_session;
        using base_type = session;

        using socket_type = boost::asio::ip::tcp::socket;

        // WebSocket 流的类型
        using tcp_stream_type = boost::beast::tcp_stream;
        using web_socket_stream_type = beast_websocket::stream<tcp_stream_type>;

        websocket_session(socket_type socket,
                          int64_t session_id,
                          const std::string& game_server_id,
                          session_callback session_callback);

        // 启动会话处理协程
        void start() override;

        [[nodiscard]] void_awaitable_type run() override;

        void write(buffer_guard data) override;

    private:
        using session_write_shared_ptr = std::shared_ptr<session_write>;

        void set_option(const std::string& game_server_id);

        void close_web_socket();

        web_socket_stream_type web_socket_;
        session_write_shared_ptr websocket_session_write_;
    };
}


#pragma once

#include "session_listener.h"
#include "websocket_session.h"

namespace celeritas
{
    class websocket_listener final : public session_listener
    {
    public:
        using class_type = websocket_listener;
        using base_type = session_listener;

        websocket_listener(io_context_type& io_context,
                           network_message_callback_weak_ptr callback,
                           std::string game_server_id,
                           int port);

        ~websocket_listener() noexcept override = default;

        websocket_listener(const websocket_listener& rhs) = delete;

        websocket_listener& operator=(const websocket_listener& rhs) = delete;

        websocket_listener(websocket_listener&& rhs) noexcept = delete;

        websocket_listener& operator=(websocket_listener&& rhs) noexcept = delete;

        // 停止监听器
        void stop() override;

        // 协程：异步接受新连接
        [[nodiscard]] void_awaitable_type accept_connections() override;

    private:
        using acceptor_type = boost::asio::ip::tcp::acceptor;
        using socket_type = websocket_session::socket_type;

        void set_option(int port);

        // 协程：处理单个连接
        [[nodiscard]] void_awaitable_type handle_connection();

        void start_new_session(socket_type socket);

        acceptor_type acceptor_;
    };
}
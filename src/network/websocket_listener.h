#pragma once

#include "listener.h"
#include "websocket_session.h"

#include <boost/asio.hpp>
#include <map>

namespace celeritas
{
    class websocket_listener final : public listener
    {
    public:
        using class_type = websocket_listener;
        using base_type = listener;

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

        void remove_session(int64_t session_id) override;

        // 协程：异步接受新连接
        [[nodiscard]] void_awaitable_type accept_connections() override;

    private:
        using acceptor_type = boost::asio::ip::tcp::acceptor;
        using session_shared_ptr = std::shared_ptr<session>;
        using session_type_container_type = std::map<int64_t, session_shared_ptr>;

        void set_option(int port);

        // 协程：处理单个连接
        [[nodiscard]] void_awaitable_type handle_connection();

        acceptor_type acceptor_;
        bool is_running_;
        session_type_container_type sessions_;
        long session_id_;
    };
}
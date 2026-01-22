#pragma once

#include "listener_accept.h"
#include "network/core/listener.h"

namespace celeritas
{
    class http_listener final : public listener
    {
    public:
        using class_type = http_listener;
        using base_type = listener;

        http_listener(const any_io_executor& any_io_executor,
                      network_message_callback_weak_ptr callback,
                      std::string game_server_id,
                      int port,
                      server_network_type server_network_type);

        void stop() override;

        [[nodiscard]] void_awaitable_type accept_connections() override;

        [[nodiscard]] session_shared_ptr get_session(int64_t id) override;

        [[nodiscard]] bool write(const std::string& server_type, const std::string& instance_id, const header& header, const protobuf_message& request) override;

    private:
        using acceptor_type = boost::asio::ip::tcp::acceptor;
        using listener_accept_shared_ptr = std::shared_ptr<listener_accept>;

        void set_option(int port);

        acceptor_type acceptor_;
        listener_accept_shared_ptr listener_accept_;
    };
}
#pragma once

#include "common/buffer_pool_data.h"
#include "common/common_fwd.h"
#include "network_fwd.h"

#include <boost/asio/awaitable.hpp>
#include <boost/asio/local/stream_protocol.hpp>

namespace celeritas
{
    class ipc_session : public std::enable_shared_from_this<ipc_session>
    {
    public:
        using class_type = ipc_session;
        using socket_type = boost::asio::local::stream_protocol::socket;
        using awaitable_type = boost::asio::awaitable<void>;
        using message_handler_type = std::function<void(const message_header&, buffer_guard)>;

        ipc_session(socket_type socket, message_handler_type handler);

        void start();

        [[nodiscard]] awaitable_type write(buffer_pool_data& data);

    private:
        [[nodiscard]] awaitable_type handle_read();

    private:
        socket_type socket_;
        message_handler_type message_handler_;
    };
}

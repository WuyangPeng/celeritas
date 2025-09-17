#pragma once

#include <boost/asio/awaitable.hpp>
#include <boost/asio/local/stream_protocol.hpp>

namespace celeritas
{
    class ipc_session : public std::enable_shared_from_this<ipc_session>
    {
    public:
        using class_type = ipc_session;

        explicit ipc_session(boost::asio::local::stream_protocol::socket socket);

        void start();

        [[nodiscard]] boost::asio::awaitable<void> write(const std::vector<char>& data);

    private:
        [[nodiscard]] boost::asio::awaitable<void> handle_read();

    private:
        boost::asio::local::stream_protocol::socket socket_;
    };
}



#pragma once

#include "network/session_callback.h"
#include "network/session_run.h"

#include <boost/asio/buffer.hpp>

namespace celeritas
{
    template <typename SocketType>
    class tcp_session_run : public session_run
    {
    public:
        using class_type = tcp_session_run;
        using base_type = session_run;
        using socket_type = SocketType;

        tcp_session_run(socket_type& socket, session_callback session_callback);

        void start() override;

        [[nodiscard]] void_awaitable_type run() override;

    private:
        using read_awaitable_type = boost::asio::awaitable<size_t>;

        [[nodiscard]] void_awaitable_type handle_one_message();

        // 协程：处理带超时的异步读取操作
        [[nodiscard]] read_awaitable_type read_data_with_timeout(boost::asio::mutable_buffer buffer);

        socket_type& socket_;
        session_callback session_callback_;
    };
}



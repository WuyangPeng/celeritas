#pragma once

#include "network/session_helper/session_callback.h"
#include "network/session_helper/session_run.h"

#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>

namespace celeritas
{
    template <typename SocketType>
    class tcp_session_run final : public session_run
    {
    public:
        using class_type = tcp_session_run;
        using base_type = session_run;
        using socket_type = SocketType;

        tcp_session_run(socket_type& socket, int64_t session_id, session_callback session_callback);

        void do_start() override;

        [[nodiscard]] void_awaitable_type run() override;

        void send_offline_message() override;

    private:
        using read_awaitable_type = boost::asio::awaitable<size_t>;
        using buffer_guard_optional_type = std::optional<buffer_guard>;
        using steady_timer_type = boost::asio::steady_timer;
        using mutable_buffer_type = boost::asio::mutable_buffer;
        using cancellation_signal_type = boost::asio::cancellation_signal;

        [[nodiscard]] auto setup_timeout_cancellation_slot(steady_timer_type& steady_timer, cancellation_signal_type& cancel_signal);

        [[nodiscard]] void_awaitable_type handle_one_message();

        // 协程：处理带超时的异步读取操作
        [[nodiscard]] read_awaitable_type read_data_with_timeout(mutable_buffer_type buffer);

        void call_back(const message_header& message_header, buffer_guard buffer_guard);

        [[nodiscard]] buffer_guard_optional_type get_buffer_guard(message_header& message_header);

        socket_type& socket_;
        int64_t session_id_;
        session_callback session_callback_;
    };
}



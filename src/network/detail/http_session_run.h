#pragma once

#include "network/session_callback.h"
#include "network/session_run.h"

#include <boost/asio.hpp>

namespace celeritas
{
    class http_session_run final : public session_run
    {
    public:
        using class_type = http_session_run;
        using base_type = session_run;
        using socket_type = boost::asio::ip::tcp::socket;

        http_session_run(socket_type& socket, int64_t session_id, session_callback session_callback);

        void start() override;

        [[nodiscard]] void_awaitable_type run() override;

    private:
        void close_socket();

        [[nodiscard]] void_awaitable_type handle_one_message();

        socket_type& socket_;
        int64_t session_id_;
        session_callback session_callback_;
    };
}
#pragma once

#include "network/session_helper/session_callback.h"
#include "network/session_helper/session_run.h"

#include <boost/beast.hpp>

namespace celeritas
{
    namespace beast_websocket = boost::beast::websocket;

    class websocket_session_run final : public session_run
    {
    public:
        using class_type = websocket_session_run;
        using base_type = session_run;
        using web_socket_stream_type = boost::beast::websocket::stream<boost::beast::tcp_stream>;

        websocket_session_run(web_socket_stream_type& web_socket, int64_t session_id, session_callback session_callback);

        void do_start() override;

        [[nodiscard]] void_awaitable_type run() override;

    private:
        void close_web_socket();

        web_socket_stream_type& web_socket_;
        int64_t session_id_;
        session_callback session_callback_;
    };
}
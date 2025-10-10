#pragma once

#include "network/session_write.h"

#include <boost/beast.hpp>

namespace celeritas
{
    class http_session_write final : public session_write
    {
    public:
        using class_type = http_session_write;
        using base_type = session_write;
        using socket_type = boost::asio::ip::tcp::socket;

        explicit http_session_write(socket_type& socket);

        // 向客户端发送消息
        void write(buffer_guard data) override;

        [[nodiscard]] void_awaitable_type do_write() override;

    private:
        socket_type& socket_;
    };
}

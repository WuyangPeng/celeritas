#pragma once

#include "network/session_write.h"

#include <boost/beast.hpp>
#include <deque>

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
        using buffer_guard_container_type = std::deque<buffer_guard>;
        using buffer_guard_optional_type = std::optional<buffer_guard>;
        using bool_awaitable_type = boost::asio::awaitable<bool>;

        [[nodiscard]] bool_awaitable_type do_one_write();

        [[nodiscard]] buffer_guard_optional_type get_next_write_buffer();

        void co_spawn_write();

        [[nodiscard]] bool write_buffer_guard(buffer_guard data);

        socket_type& socket_;
        buffer_guard_container_type write_queue_;
        std::mutex write_mutex_;
    };
}

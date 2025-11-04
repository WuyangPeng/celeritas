#pragma once

#include "network/session_write.h"

#include <boost/beast.hpp>
#include <deque>

namespace celeritas
{
    class http_session_write : public session_write
    {
    public:
        using class_type = http_session_write;
        using base_type = session_write;
        using socket_type = boost::asio::ip::tcp::socket;

        explicit http_session_write(socket_type& socket, std::string host);

        // 向客户端发送消息
        void write(buffer_guard data) override;

        [[nodiscard]] void_awaitable_type do_write() override;

        [[nodiscard]] void_awaitable_type write_immediately(buffer_guard data) override;

        [[nodiscard]] bool is_full() override;

    protected:
        using bool_awaitable_type = boost::asio::awaitable<bool>;

        [[nodiscard]] bool_awaitable_type do_one_write_response();

        [[nodiscard]] bool_awaitable_type do_one_write_request(const std::string& path);

    private:
        using buffer_guard_container_type = std::deque<buffer_guard>;
        using buffer_guard_optional_type = std::optional<buffer_guard>;

        [[nodiscard]] virtual bool_awaitable_type do_one_write() = 0;

        [[nodiscard]] buffer_guard_optional_type get_next_write_buffer();

        void co_spawn_write();

        bool write_buffer_guard(buffer_guard data);

        socket_type& socket_;
        buffer_guard_container_type write_queue_;
        std::mutex write_mutex_;
        std::string host_;
    };
}

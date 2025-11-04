#pragma once

#include "session.h"

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <memory>

namespace celeritas
{
    class http_session final : public session
    {
    public:
        using class_type = http_session;
        using base_type = session;
        using socket_type = boost::asio::ip::tcp::socket;

        http_session(socket_type socket,
                     int64_t session_id,
                     std::string game_server_id,
                     session_callback session_callback,
                     bool is_server,
                     std::string host,
                     std::string path);

        ~http_session() noexcept override;

        http_session(const http_session& rhs) = delete;

        http_session& operator=(const http_session& rhs) = delete;

        http_session(http_session&& rhs) noexcept = delete;

        http_session& operator=(http_session&& rhs) noexcept = delete;

        // 启动会话处理协程
        void start() override;

        [[nodiscard]] void_awaitable_type start_awaitable() override;

        [[nodiscard]] bool is_open() const override;

        void stop() override;

    private:
        using session_run_shared_ptr = std::shared_ptr<session_run>;
        using session_write_shared_ptr = std::shared_ptr<session_write>;

        void do_write(buffer_guard data) override;

        [[nodiscard]] static session_write_shared_ptr get_session_write(socket_type& socket, bool is_server, std::string host, std::string path);

        [[nodiscard]] void_awaitable_type do_write_immediately(buffer_guard data) override;

        socket_type socket_;
        session_run_shared_ptr http_run_;
        session_write_shared_ptr http_write_;
        std::string game_server_id_;
    };
}


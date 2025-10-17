#pragma once

#include "network_message_callback.h"
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
                     session_callback session_callback);

        // 启动会话处理协程
        void start() override;

        [[nodiscard]] bool is_open() const override;

    private:
        using session_run_shared_ptr = std::shared_ptr<session_run>;
        using session_write_shared_ptr = std::shared_ptr<session_write>;

        void write(buffer_guard data) override;

    private:
        socket_type socket_;
        session_run_shared_ptr http_run_;
        session_write_shared_ptr http_write_;
        std::string game_server_id_;
    };
}


#pragma once

#include "session.h"
#include "common/buffer_guard.h"

namespace celeritas
{
    template <typename SocketType>
    class session_base final : public session
    {
    public:
        using class_type = session_base;
        using base_type = session;
        using socket_type = SocketType;

        // 接受一个已连接的 socket
        session_base(socket_type socket,
                     long session_id,
                     std::string game_server_id,
                     session_callback session_callback);

        ~session_base() noexcept override;

        session_base(const session_base& rhs) = delete;

        session_base& operator=(const session_base& rhs) = delete;

        session_base(session_base&& rhs) noexcept = delete;

        session_base& operator=(session_base&& rhs) noexcept = delete;

        // 启动会话处理协程
        void start() override;

        [[nodiscard]] bool is_open() const override;

        void stop() override;

    private:
        using session_write_shared_ptr = std::shared_ptr<session_write>;
        using session_run_shared_ptr = std::shared_ptr<session_run>;

        // 向客户端发送消息
        void do_write(buffer_guard data) override;

    private:
        socket_type socket_;
        session_write_shared_ptr session_write_;
        session_run_shared_ptr session_run_;
        std::string game_server_id_;
    };
}

#pragma once

#include "network/core/session_base.h"
#include "common/buffer/buffer_guard.h"

namespace celeritas
{
    template <typename SocketType>
    class generic_session final : public session_base
    {
    public:
        using class_type = generic_session;
        using base_type = session_base;
        using socket_type = SocketType;

        // 接受一个已连接的 socket
        generic_session(socket_type socket,
                        server_network_type server_network_type,
                        long session_id,
                        std::string game_server_id,
                        session_callback session_callback);

        ~generic_session() noexcept override;

        generic_session(const generic_session& rhs) = delete;

        generic_session& operator=(const generic_session& rhs) = delete;

        generic_session(generic_session&& rhs) noexcept = delete;

        generic_session& operator=(generic_session&& rhs) noexcept = delete;

        // 启动会话处理协程
        void start() override;

        [[nodiscard]] void_awaitable_type start_awaitable() override;

        void stop() override;

        [[nodiscard]] bool is_open() const override;

        [[nodiscard]] bool is_full() const override;

        [[nodiscard]] any_io_executor get_any_io_executor() override;

    private:
        using session_write_shared_ptr = std::shared_ptr<session_write>;
        using session_run_shared_ptr = std::shared_ptr<session_run>;

        // 向客户端发送消息
        void do_write(buffer_guard data) override;

        [[nodiscard]] void_awaitable_type do_write_immediately(buffer_guard data) override;

        socket_type socket_;
        session_write_shared_ptr session_write_;
        session_run_shared_ptr session_run_;
        std::string game_server_id_;
    };
}

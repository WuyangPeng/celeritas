#pragma once

#include "session_callback.h"
#include "common/buffer_guard.h"
#include "common/session.h"
#include "message/header.h"

#include <memory>

namespace celeritas
{
    class session_base : public session
    {
    public:
        using class_type = session_base;
        using base_type = session;

        session_base(int64_t session_id, session_callback session_callback);

        // 启动会话处理协程
        virtual void start() = 0;

        [[nodiscard]] virtual void_awaitable_type start_awaitable() = 0;

        void write(const header& header, const protobuf_message_type& response) override;

        void write(const std::string& response) override;

        [[nodiscard]] void_awaitable_type write_immediately(const std::string& response) override;

        [[nodiscard]] int64_t get_session_id() const noexcept override;

        [[nodiscard]] virtual bool is_open() const = 0;

        [[nodiscard]] virtual bool is_full() const = 0;

        void remove_session() override;

    protected:
        using message_shared_ptr = header::message_shared_ptr;
        using network_message_callback_weak_ptr = session_callback::network_message_callback_weak_ptr;

        [[nodiscard]] network_message_callback_weak_ptr get_network_message_callback();

        [[nodiscard]] session_callback get_session_callback() const;

    private:
        virtual void do_write(buffer_guard data) = 0;

        virtual void_awaitable_type do_write_immediately(buffer_guard data) = 0;

        int64_t session_id_;
        session_callback session_callback_;
    };
}
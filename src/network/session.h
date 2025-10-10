#pragma once

#include "session_callback.h"
#include "common/buffer_guard.h"

#include <boost/asio/awaitable.hpp>
#include <memory>

namespace celeritas
{
    class session : public std::enable_shared_from_this<session>
    {
    public:
        using class_type = session;
        using void_awaitable_type = boost::asio::awaitable<void>;

        session(int64_t session_id, session_callback session_callback);

        virtual ~session() noexcept = default;

        session(const session& rhs) = default;

        session& operator=(const session& rhs) = default;

        session(session&& rhs) noexcept = default;

        session& operator=(session&& rhs) noexcept = default;

        // 启动会话处理协程
        virtual void start() = 0;

        virtual void write(buffer_guard data) = 0;

        [[nodiscard]] int64_t get_session_id() const noexcept;

    protected:
        using network_message_callback_weak_ptr = session_callback::network_message_callback_weak_ptr;

        void remove_session();

        [[nodiscard]] network_message_callback_weak_ptr get_network_message_callback();

        [[nodiscard]] session_callback get_session_callback() const;

    private:
        int64_t session_id_;
        session_callback session_callback_;
    };
}
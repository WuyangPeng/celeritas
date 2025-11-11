#pragma once

#include "session_callback.h"
#include "common/buffer_guard.h"
#include "message/header.h"

#include <boost/asio/awaitable.hpp>
#include <memory>

namespace celeritas
{
    class session_base : public std::enable_shared_from_this<session_base>
    {
    public:
        using class_type = session_base;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using protobuf_message_type = google::protobuf::Message;

        session_base(int64_t session_id, session_callback session_callback);

        virtual ~session_base() noexcept = default;

        session_base(const session_base& rhs) = default;

        session_base& operator=(const session_base& rhs) = default;

        session_base(session_base&& rhs) noexcept = default;

        session_base& operator=(session_base&& rhs) noexcept = default;

        // 启动会话处理协程
        virtual void start() = 0;

        [[nodiscard]] virtual void_awaitable_type start_awaitable() = 0;

        virtual void stop() = 0;

        void write(const header& header, const protobuf_message_type& response);

        void write(const std::string& response);

        [[nodiscard]] void_awaitable_type write_immediately(const std::string& response);

        [[nodiscard]] int64_t get_session_id() const noexcept;

        [[nodiscard]] virtual bool is_open() const = 0;

        [[nodiscard]] virtual bool is_full() const = 0;

    protected:
        using network_message_callback_weak_ptr = session_callback::network_message_callback_weak_ptr;
        using message_shared_ptr = header::message_shared_ptr;

        void remove_session();

        [[nodiscard]] network_message_callback_weak_ptr get_network_message_callback();

        [[nodiscard]] session_callback get_session_callback() const;

    private:
        virtual void do_write(buffer_guard data) = 0;

        virtual void_awaitable_type do_write_immediately(buffer_guard data) = 0;

        int64_t session_id_;
        session_callback session_callback_;
    };
}
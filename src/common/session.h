#pragma once

#include "message/message_fwd.h"

#include <boost/asio/awaitable.hpp>
#include <google/protobuf/message.h>

#include <memory>

namespace celeritas
{
    class session : public std::enable_shared_from_this<session>
    {
    public:
        using class_type = session;
        using protobuf_message_type = google::protobuf::Message;
        using void_awaitable_type = boost::asio::awaitable<void>;

        session() noexcept = default;

        virtual ~session() noexcept = default;

        session(const session& rhs) noexcept = default;

        session& operator=(const session& rhs) noexcept = default;

        session(session&& rhs) noexcept = default;

        session& operator=(session&& rhs) noexcept = default;

        virtual void stop() = 0;

        virtual void write(const header& header, const protobuf_message_type& response) = 0;

        virtual void write(const std::string& response) = 0;

        [[nodiscard]] virtual void_awaitable_type write_immediately(const std::string& response) = 0;
    };
}
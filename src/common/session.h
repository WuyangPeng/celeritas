#pragma once

#include "message/message_fwd.h"

#include <google/protobuf/message.h>

#include <memory>

namespace celeritas
{
    class session : public std::enable_shared_from_this<session>
    {
    public:
        using class_type = session;
        using protobuf_message_type = google::protobuf::Message;

        session() noexcept = default;

        virtual ~session() noexcept = default;

        session(const session& rhs) noexcept = default;

        session& operator=(const session& rhs) noexcept = default;

        session(session&& rhs) noexcept = default;

        session& operator=(session&& rhs) noexcept = default;

        virtual void stop() = 0;

        virtual void write(const header& header, const protobuf_message_type& response) = 0;

        virtual void write(const std::string& response) = 0;
    };
}
#pragma once

#include "common/buffer_guard.h"
#include "message/header.h"
#include "network/network_fwd.h"

namespace celeritas
{
    class write_protobuf_message
    {
    public:
        using class_type = write_protobuf_message;
        using protobuf_message_type = google::protobuf::Message;
        using session_shared_ptr = std::shared_ptr<session>;
        using buffer_guard_optional_type = std::optional<buffer_guard>;

        write_protobuf_message(const session_shared_ptr& session, const header& header, const protobuf_message_type& response);

        [[nodiscard]] buffer_guard_optional_type get_buffer_guard() const;

    private:
        using message_shared_ptr = header::message_shared_ptr;

        [[nodiscard]] bool write(const message_shared_ptr& header_request,
                                 size_t header_size,
                                 buffer_guard& buffer_guard) const;

        const session_shared_ptr& session_;
        const header& header_;
        const protobuf_message_type& response_;
    };
}
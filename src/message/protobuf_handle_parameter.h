#pragma once

#include "header.h"
#include "common/common_fwd.h"

#include <google/protobuf/message.h>

namespace celeritas
{
    class protobuf_handle_parameter
    {
    public:
        using class_type = protobuf_handle_parameter;
        using protobuf_message = google::protobuf::Message;
        using protobuf_message_shared_ptr = std::shared_ptr<protobuf_message>;
        using session_shared_ptr = std::shared_ptr<session>;
        using resource_loader_shared_ptr = std::shared_ptr<resource_loader_base>;

        protobuf_handle_parameter(const header& header, protobuf_message_shared_ptr request_message, const session_shared_ptr& session, const resource_loader_shared_ptr& resource_loader);

        void write(const protobuf_message& response) const;

        void write(const std::string& server_type, const protobuf_message& request) const;

        [[nodiscard]] protobuf_message_shared_ptr get_protobuf_message() const;

    private:
        using session_weak_ptr = std::weak_ptr<session>;
        using resource_loader_weak_ptr = std::weak_ptr<resource_loader_base>;

        header header_;
        protobuf_message_shared_ptr request_message_;
        session_weak_ptr session_;
        resource_loader_weak_ptr resource_loader_;
    };
}

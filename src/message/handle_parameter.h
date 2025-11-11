#pragma once

#include "header.h"
#include "initializer/initializer_fwd.h"
#include "network/network_fwd.h"
#include "proto/celeritas.pb.h"

#include <google/protobuf/message.h>

namespace celeritas
{
    class handle_parameter
    {
    public:
        using class_type = handle_parameter;
        using protobuf_message = google::protobuf::Message;
        using protobuf_message_shared_ptr = std::shared_ptr<google::protobuf::Message>;
        using session_shared_ptr = std::shared_ptr<session_base>;
        using resource_loader_shared_ptr = std::shared_ptr<resource_loader>;

        handle_parameter(const header& header, protobuf_message_shared_ptr request_message, const session_shared_ptr& session, const resource_loader_shared_ptr& resource_loader);

        void write(const protobuf_message& response) const;

        void write(const std::string& server_type, const protobuf_message& request) const;

        [[nodiscard]] protobuf_message_shared_ptr get_protobuf_message() const;

    private:
        using session_weak_ptr = std::weak_ptr<session_base>;
        using resource_loader_weak_ptr = std::weak_ptr<resource_loader>;

        header header_;
        protobuf_message_shared_ptr request_message_;
        session_weak_ptr session_;
        resource_loader_weak_ptr resource_loader_;
    };
}

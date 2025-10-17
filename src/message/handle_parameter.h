#pragma once

#include "header.h"
#include "network/network_fwd.h"
#include "proto/response.pb.h"

#include <google/protobuf/message.h>

namespace celeritas
{
    class handle_parameter
    {
    public:
        using class_type = handle_parameter;
        using protobuf_message_shared_ptr = std::shared_ptr<google::protobuf::Message>;
        using session_shared_ptr = std::shared_ptr<session>;

        handle_parameter(const header& header, const protobuf_message_shared_ptr& request_message, const session_shared_ptr& session);

        void write(const proto::response& response) const;

    private:
        header header_;
        protobuf_message_shared_ptr request_message_;
        session_shared_ptr session_;
    };
}

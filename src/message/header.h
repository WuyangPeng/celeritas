#pragma once

#include "proto/common/common.pb.h"

namespace celeritas
{
    class header
    {
    public:
        using class_type = header;
        using message_shared_ptr = std::shared_ptr<google::protobuf::Message>;
        using empty_message_header_type = proto::common::empty_message_header;
        using server_message_header_type = proto::common::server_message_header;
        using client_message_header_type = proto::common::client_message_header;
        using gateway_message_header_type = proto::common::gateway_message_header;
        using to_gateway_message_header_type = proto::common::to_gateway_message_header;

        explicit header(const empty_message_header_type& empty_message_header);

        explicit header(const server_message_header_type& server_message_header);

        explicit header(const client_message_header_type& client_message_header);

        explicit header(const gateway_message_header_type& gateway_message_header);

        explicit header(const to_gateway_message_header_type& to_gateway_message_header);

        [[nodiscard]] message_shared_ptr get_message() const;

    private:
        int32_t rpc = 0;
        int64_t user_id = 0;
        int32_t code = 0;
    };
}

#pragma once

#include "proto/common/common.pb.h"

namespace celeritas
{
    class header
    {
    public:
        using class_type = header;
        using message_shared_ptr = std::shared_ptr<google::protobuf::Message>;

        explicit header(const proto::empty_message_header& empty_message_header);

        explicit header(const proto::server_message_header& server_message_header);

        explicit header(const proto::client_message_header& client_message_header);

        [[nodiscard]] message_shared_ptr get_message() const;

    private:
        int32_t rpc = 0;
        int64_t user_id = 0;
        int32_t code = 0;
    };
}

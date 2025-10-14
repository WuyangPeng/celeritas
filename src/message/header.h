#pragma once

#include "proto/common/common.pb.h"

namespace celeritas
{
    class header
    {
    public:
        using class_type = header;

        explicit header(const proto::server_message_header& server_message_header);

        explicit header(const proto::client_message_header& client_message_header);

    private:
        int32_t rpc = 0;
        int64_t user_id = 0;
        int32_t code = 0;
    };
}

#pragma once

#include "boost/log/sources/record_ostream.hpp"
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

        header() noexcept = default;

        header(int32_t rpc, int32_t code);

        header(int32_t rpc, int64_t user_id, int32_t code);

        header(int32_t rpc, int64_t user_id);

        explicit header(int64_t user_id);

        explicit header(const empty_message_header_type& empty_message_header);

        explicit header(const server_message_header_type& server_message_header);

        explicit header(const client_message_header_type& client_message_header);

        explicit header(const gateway_message_header_type& gateway_message_header);

        explicit header(const to_gateway_message_header_type& to_gateway_message_header);

        [[nodiscard]] message_shared_ptr get_message() const;

        [[nodiscard]] int32_t get_rpc() const;

        [[nodiscard]] int64_t get_user_id() const;

        [[nodiscard]] int32_t get_code() const;

    private:
        [[nodiscard]] message_shared_ptr get_to_gateway_message() const;

        [[nodiscard]] message_shared_ptr get_gateway_message() const;

        [[nodiscard]] message_shared_ptr get_server_message() const;

        [[nodiscard]] message_shared_ptr get_client_message() const;

        [[nodiscard]] message_shared_ptr get_empty_message() const;

        int32_t rpc_ = 0;
        int64_t user_id_ = 0;
        int32_t code_ = 0;
    };
}

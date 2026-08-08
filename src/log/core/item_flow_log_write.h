#pragma once

#include "message/message_fwd.h"
#include "message/parameters/protobuf_handle_parameter.h"
#include "proto/service/log.pb.h"

#include <boost/asio/awaitable.hpp>
#include <memory>

namespace celeritas
{
    class item_flow_log_write
    {
    public:
        using class_type = item_flow_log_write;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using protobuf_handle_parameter_shared_ptr = std::shared_ptr<protobuf_handle_parameter>;

        item_flow_log_write(protobuf_handle_parameter_shared_ptr protobuf_handle_parameter, const proto::service::service_log_request& request);

        [[nodiscard]] void_awaitable_type response() const;

        void send_error_message(game_error_type game_error_type) const;

    private:
        protobuf_handle_parameter_shared_ptr protobuf_handle_parameter_;
        const proto::service::service_log_request& request_;
    };
}

#pragma once

#include "player/component/player_service_base.h"
#include "proto/client/player/heartbeat.pb.h"

#include <boost/asio/awaitable.hpp>

namespace celeritas
{
    class heartbeat : public player_service_base
    {
    public:
        using class_type = heartbeat;
        using base_type = player_service_base;
        using request_type = proto::client::heartbeat_request;

        heartbeat(protobuf_handle_parameter_shared_ptr handle_parameter, player_state_shared_ptr player_state, request_type request);

        [[nodiscard]] void_awaitable_type response() override;

    private:
        void send_heartbeat();
    };
}
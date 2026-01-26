#pragma once

#include "player_role_component.h"
#include "player/component/player_service_base.h"
#include "proto/client/player/role.pb.h"

#include <boost/asio/awaitable.hpp>

namespace celeritas
{
    class change_name : public player_service_base
    {
    public:
        using class_type = change_name;
        using base_type = player_service_base;
        using request_type = proto::client::change_role_name_request;

        explicit change_name(protobuf_handle_parameter_shared_ptr handle_parameter,
                             player_state_shared_ptr player_state,
                             const request_type& request);

        [[nodiscard]] void_awaitable_type response() override;

    private:
        using player_role_component_shared_ptr = std::shared_ptr<player_role_component>;

        request_type request_;
        player_role_component_shared_ptr player_role_component_;
    };
}

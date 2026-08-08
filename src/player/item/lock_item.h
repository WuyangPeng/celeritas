#pragma once

#include "player/component/player_service_base.h"
#include "proto/client/player/item.pb.h"

namespace celeritas
{
    class lock_item : public player_service_base
    {
    public:
        using class_type = lock_item;
        using base_type = player_service_base;
        using request_type = proto::client::lock_item_request;
        using lock_item_shared_ptr = std::shared_ptr<class_type>;

        [[nodiscard]] static lock_item_shared_ptr create(protobuf_handle_parameter_shared_ptr handle_parameter,
                                                         player_state_shared_ptr player_state,
                                                         request_type request);

        lock_item(protobuf_handle_parameter_shared_ptr handle_parameter,
                  player_state_shared_ptr player_state,
                  request_type request);

        [[nodiscard]] void_awaitable_type response() override;

    private:
        using player_item_component_shared_ptr = std::shared_ptr<player_item_component>;

        void send_lock_item_response();

        request_type request_;
        player_item_component_shared_ptr player_item_component_;
    };
}

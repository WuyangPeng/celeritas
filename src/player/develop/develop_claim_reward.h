#pragma once

#include "player/component/player_service_base.h"
#include "proto/client/player/develop.pb.h"

namespace celeritas
{
    class develop_claim_reward : public player_service_base
    {
    public:
        using class_type = develop_claim_reward;
        using base_type = player_service_base;
        using request_type = proto::client::develop_claim_reward_request;
        using develop_claim_reward_shared_ptr = std::shared_ptr<class_type>;

        [[nodiscard]] static develop_claim_reward_shared_ptr create(protobuf_handle_parameter_shared_ptr handle_parameter,
                                                                     player_state_shared_ptr player_state,
                                                                     request_type request);

        develop_claim_reward(protobuf_handle_parameter_shared_ptr handle_parameter,
                             player_state_shared_ptr player_state,
                             request_type request);

        [[nodiscard]] void_awaitable_type response() override;

    private:
        using player_item_component_shared_ptr = std::shared_ptr<player_item_component>;
        using player_develop_component_shared_ptr = std::shared_ptr<player_develop_component>;

        request_type request_;
        player_item_component_shared_ptr player_item_component_;
        player_develop_component_shared_ptr player_develop_component_;
    };
}

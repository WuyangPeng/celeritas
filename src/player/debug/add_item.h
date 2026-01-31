#pragma once

#include "debug_base.h"

namespace celeritas
{
    class add_item : public debug_base
    {
    public:
        using class_type = add_item;
        using base_type = debug_base;

        add_item(protobuf_handle_parameter_shared_ptr handle_parameter,
                 player_state_shared_ptr player_state,
                 request_type request);

    private:
        using player_item_component_shared_ptr = std::shared_ptr<player_item_component>;

        [[nodiscard]] game_error_awaitable_type do_response() override;

        player_item_component_shared_ptr player_item_component_;
    };
}
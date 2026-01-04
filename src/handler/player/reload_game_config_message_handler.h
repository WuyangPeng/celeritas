#pragma once

#include "message/handler_base/concrete_message_handler.h"
#include "proto/service/player.pb.h"

namespace celeritas
{
    class reload_game_config_message_handler final : public concrete_message_handler<proto::service::reload_game_config_request>
    {
    public:
        using class_type = reload_game_config_message_handler;
        using base_type = concrete_message_handler;

    protected:
        [[nodiscard]] bool handle_concrete(const protobuf_handle_parameter_shared_ptr& handle_parameter,
                                           const message_type& current_message,
                                           const message_registry_weak_ptr& message_registry) override;
    };
}




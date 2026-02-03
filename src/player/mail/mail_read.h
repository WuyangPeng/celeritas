#pragma once

#include "player/component/player_service_base.h"
#include "proto/client/player/mail.pb.h"

namespace celeritas
{
    class mail_read : public player_service_base
    {
    public:
        using class_type = mail_read;
        using base_type = player_service_base;
        using request_type = proto::client::mail_read_request;
        using mail_read_shared_ptr = std::shared_ptr<class_type>;

        [[nodiscard]] static mail_read_shared_ptr create(protobuf_handle_parameter_shared_ptr handle_parameter,
                                                         player_state_shared_ptr player_state,
                                                         request_type request);

        mail_read(protobuf_handle_parameter_shared_ptr handle_parameter,
                  player_state_shared_ptr player_state,
                  request_type request);

        [[nodiscard]] void_awaitable_type response() override;

    private:
        using player_mail_component_shared_ptr = std::shared_ptr<player_mail_component>;

        request_type request_;
        player_mail_component_shared_ptr player_mail_component_;
    };
}
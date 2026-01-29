#pragma once

#include "player/component/player_service_base.h"
#include "proto/client/player/debug.pb.h"

namespace celeritas
{
    class debug_base : public player_service_base
    {
    public:
        using class_type = debug_base;
        using base_type = player_service_base;
        using request_type = proto::client::debug_request;
        using debug_shared_ptr = std::shared_ptr<class_type>;

        [[nodiscard]] static debug_shared_ptr create(protobuf_handle_parameter_shared_ptr handle_parameter,
                                                     player_state_shared_ptr player_state,
                                                     request_type request);

        debug_base(protobuf_handle_parameter_shared_ptr handle_parameter,
                   player_state_shared_ptr player_state,
                   request_type request);

        [[nodiscard]] void_awaitable_type response() override;

    protected:
        using game_error_awaitable_type = boost::asio::awaitable<game_error_type>;

        [[nodiscard]] virtual game_error_awaitable_type do_response() = 0;

        [[nodiscard]] int64_t get_id() const;

        [[nodiscard]] int64_t get_parameter() const;

    private:
        request_type request_;
    };
}
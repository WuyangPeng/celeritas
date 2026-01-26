#include "player_service_base.h"

celeritas::player_service_base::player_service_base(protobuf_handle_parameter_shared_ptr handle_parameter,
                                                    player_state_shared_ptr player_state)
    : handle_parameter_{ std::move(handle_parameter) }, player_state_{ std::move(player_state) }
{
}

celeritas::player_service_base::player_state_shared_ptr celeritas::player_service_base::get_player_state()
{
    return player_state_;
}

int celeritas::player_service_base::get_rpc() const
{
    return handle_parameter_->get_rpc();
}
#include "player_service_base.h"
#include "config/game/game_config.h"

celeritas::player_service_base::player_service_base(protobuf_handle_parameter_shared_ptr handle_parameter,
                                                    player_state_shared_ptr player_state)
    : handle_parameter_{ std::move(handle_parameter) },
      player_state_{ std::move(player_state) },
      game_tables_{ game_config::get_instance().get_game_tables() }
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

celeritas::player_service_base::const_game_tables_shared_ptr celeritas::player_service_base::get_game_tables() const
{
    return game_tables_;
}

celeritas::player_service_base::const_app_config_shared_ptr celeritas::player_service_base::get_config() const
{
    return handle_parameter_->get_app_config();
}
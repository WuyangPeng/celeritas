#include "reload_game_config_message_handler.h"
#include "config/aggregate/app_config.h"
#include "config/game/game_config.h"
#include "database/config/config_manager.h"
#include "message/protobuf_handle_parameter.h"
#include "proto/celeritas.pb.h"

bool celeritas::reload_game_config_message_handler::handle_concrete(const protobuf_handle_parameter_shared_ptr& handle_parameter,
                                                                    const message_type& current_message,
                                                                    const message_registry_weak_ptr& message_registry)
{
    if (const auto server_config = handle_parameter->get_app_config()->get_server_config();
        !server_config.is_load_game_config())
    {
        return true;
    }

    game_config::load_tables();

    proto::celeritas response{};
    response.mutable_celeritas_response()->mutable_service()->mutable_player()->mutable_reload_game_config();
    handle_parameter->write(response);

    return true;
}